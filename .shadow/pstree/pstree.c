#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
// pstree
// 2.2 描述
// 把系统中的进程按照父亲-孩子的树状结构打印到终端。

// -p 或 --show-pids: 打印每个进程的进程号。
// -n 或 --numeric-sort: 按照pid的数值从小到大顺序输出一个进程的直接孩子。
// -V 或 --version: 打印版本信息。

//hash table
struct HashEntry
{
  int key;
  struct pstreeNode *value;
  struct HashEntry *next;
};

struct HashTable
{
  struct HashEntry **buckets;
  int size;
};

struct HashTable *createHashTable(int size)
{
  struct HashTable *hashTable = (struct HashTable *)malloc(sizeof(struct HashTable));
  if (hashTable == NULL)
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  hashTable->buckets = (struct HashEntry **)calloc(size, sizeof(struct HashEntry *));
  if (hashTable->buckets == NULL)
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  hashTable->size = size;
  return hashTable;
}

int hashFunction(int key, int size)
{
  return key % size;
}

void insert(struct HashTable *hashTable, int key, struct pstreeNode *value)
{
  int index = hashFunction(key, hashTable->size);

  struct HashEntry *newEntry = (struct HashEntry *)malloc(sizeof(struct HashEntry));
  if (newEntry == NULL)
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  newEntry->key = key;
  newEntry->value = value;
  newEntry->next = NULL;

  if (hashTable->buckets[index] == NULL)
  {
    hashTable->buckets[index] = newEntry;
  }
  else
  {
    struct HashEntry *current = hashTable->buckets[index];
    while (current->next != NULL)
    {
      current = current->next;
    }
    current->next = newEntry;
  }
}


struct pstreeNode *search(struct HashTable *hashTable, int key)
{
  int index = hashFunction(key, hashTable->size);

  struct HashEntry *current = hashTable->buckets[index];
  while (current != NULL)
  {
    if (current->key == key)
    {
      return current->value;
    }
    current = current->next;
  }
  return NULL;
}

void printHashTable(struct HashTable *hashTable) {
  for (int i = 0; i < hashTable->size; i++) {
    struct HashEntry *entry = hashTable->buckets[i];
    while (entry != NULL) {
      printf("Key: %d, Value: %p\n", entry->key, entry->value);
      entry = entry->next;
    }
  }
}

void delete(struct HashTable *hashTable, int key)
{
  int index = hashFunction(key, hashTable->size);

  struct HashEntry *current = hashTable->buckets[index];
  struct HashEntry *prev = NULL;
  while (current != NULL)
  {
    if (current->key == key)
    {
      if (prev == NULL)
      {
        hashTable->buckets[index] = current->next;
      }
      else
      {
        prev->next = current->next;
      }
      free(current);
      return;
    }
    prev = current;
    current = current->next;
  }
}


// pstree
struct pstreeNode
{
  int PID;                   // /proc/[PID]
  char *processName;         // cat /proc/PID/comm
  struct pstreeNode *parent; // cat /proc/PID/status
  struct pstreeNode *firstChild;
  struct pstreeNode *nextSibling;
};

struct pstreeNode *createNode(int pid, char *name)
{
  struct pstreeNode *newNode = (struct pstreeNode *)malloc(sizeof(struct pstreeNode));
  if (newNode == NULL)
  {
    printf("内存分配失败\n");
    exit(1);
  }
  newNode->PID = pid;
  newNode->processName = name;
  newNode->parent = NULL;
  newNode->firstChild = NULL;
  newNode->nextSibling = NULL;
  return newNode;
}

// 在指定父节点下添加子节点
void addChild(struct pstreeNode *parent, struct pstreeNode *child, bool numericSort)
{
  child->parent = parent;
  if (parent->firstChild == NULL){
    parent->firstChild = child;
  }
  else {
    if (numericSort){
      // struct pstreeNode *child = parent->firstChild;
      for (struct pstreeNode *sibling = parent->firstChild; sibling != NULL; sibling = sibling->nextSibling){
        if (child->PID < sibling->PID){
          child->nextSibling = sibling;
          if (sibling == parent->firstChild){
            parent->firstChild = child;
          }
          else {
            struct pstreeNode *prev = parent->firstChild;
            while (prev->nextSibling != sibling){
              prev = prev->nextSibling;
            }
            prev->nextSibling = child;
          }
          return;
        }
      }
    }
    else
    {
      struct pstreeNode *sibling = parent->firstChild;
      while (sibling->nextSibling != NULL)
      {
        sibling = sibling->nextSibling;
      }
      sibling->nextSibling = child;
    }
  }
}

int countDirectories(const char *path)
{
  int count = 0;
  struct dirent *entry;
  DIR *dir = opendir(path);

  if (!dir)
  {
    perror("opendir");
    exit(1);
  }

  while ((entry = readdir(dir)) != NULL)
  {
    // Skip "." and ".." entries
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    {
      continue;
    }

    // Check if it's a directory
    if (entry->d_type == DT_DIR)
    {
      count++;
    }
  }

  closedir(dir);
  return count;
}

// get process name from "/proc/[PID]/comm"
char *getProcessName(struct dirent *entry)
{
  char *path = "/proc";
  char *filename = "/comm";
  char *processName = NULL;
  char *fullPath = (char *)malloc(strlen(path) + strlen(entry->d_name) + strlen(filename) + 2);
  if (fullPath == NULL)
  {
    printf("Memory allocation failed\n");
    exit(1);
  }
  sprintf(fullPath, "%s/%s%s", path, entry->d_name, filename);
  FILE *fp = fopen(fullPath, "r");
  if (fp)
  {
    // Allocate memory for processName
    processName = (char *)malloc(256);
    if (processName == NULL)
    {
      printf("Memory allocation failed\n");
      exit(1);
    }
    // 用fscanf, fgets等函数读取
    fscanf(fp, "%s", processName);
    fclose(fp);
  }
  else
  {
    // 错误处理
    printf("Error opening file %s\n", fullPath);
    exit(1);
  }
  
  free(fullPath);
  return processName;
}

// get ppid from "/proc/[PID]/status"
int getPPid(int pid) {
    char filename[256];
    snprintf(filename, sizeof(filename), "/proc/%d/status", pid);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "PPid:", 5) == 0) {
            // Extract the PPid value
            int ppid;
            sscanf(line + 5, "%d", &ppid);
            fclose(fp);
            return ppid;
        }
    }

    fclose(fp);
    return -1;  // PPid not found
}

// store process info to hash table
void storeInfo(struct dirent *entry, struct HashTable *hashTable){
  if (entry->d_type == DT_DIR && isdigit(*entry->d_name)){
    // get PID from "/proc/[PID]"
    int pid = atoi(entry->d_name);

    int ppid = getPPid(pid);
    if (ppid == -1) {
        printf("Error: PPid not found for PID %d\n", pid);
        return;
    }

    // get process name from "/proc/[PID]/comm"
    char *processName = getProcessName(entry);
    // printf("PID: %d, PPID: %d, Process Name: %s\n", pid, ppid, processName);

    insert(hashTable, pid, createNode(pid, processName));
  }
}

// traverse procfs through hash table
void traverse_procfs(void (*visit)(struct dirent *entry, struct HashTable *hashTable), struct HashTable *hashTable){
  char *path = "/proc";
  struct dirent *entry;

  DIR *dir = opendir(path);
  // 遍历进程，调用visit函数
  if (dir){
    while ((entry = readdir(dir)) != NULL){
      visit(entry, hashTable);
    }
    closedir(dir);
  }
  else{
    // Error handling
    perror("opendir");
  }
}

void examinePstree(struct pstreeNode *root) {
  if (root == NULL) {
    return;
  }

  // Print the information of the current node
  printf("PID: %d, Process Name: %s\n", root->PID, root->processName);

  // Recursively examine the children of the current node
  struct pstreeNode *child = root->firstChild;
  while (child != NULL) {
    examinePstree(child);
    child = child->nextSibling;
  }
}

struct pstreeNode * buildPstree(struct HashTable *hashTable, bool numericSort){
  for (int i = 0; i < hashTable->size; i++){
    struct HashEntry *entry = hashTable->buckets[i];
    while (entry != NULL){
      int ppid = getPPid(entry->key);
      if (ppid == -1){
        printf("Error: PPid not found for PID %d\n", entry->key);
        return NULL;
      }
      struct pstreeNode *parent = NULL;
      if (ppid != 0){
        parent = search(hashTable, ppid);
      }
      if (parent != NULL){
        addChild(parent, entry->value, numericSort);
      }
      entry = entry->next;
    }
  }
  return search(hashTable, 1);
};

void printPstree(struct pstreeNode *root, int level, bool showPids, bool numericSort) {
  if (root == NULL) {
    return;
  }
  // Print the indentation based on the level
  // for (int i = 0; i < level; i++) {
  //   printf("  ");
  // }

  // Print the process name and PID
  if (showPids) {
    printf("%s(%d)", root->processName, root->PID);
  } else {
    printf("%s", root->processName);
  }

// Print the children recursively
struct pstreeNode *child = root->firstChild;

if (child != NULL) {
  if(child->nextSibling == NULL)
    printf("───");
  else 
    printf("─┬─");

  // Calculate the length of the process name and PID
  level += strlen(root->processName);
  if(root->parent){
    level += 3; // +3 for the "─┬─"
  }
  if (showPids) {
    level += snprintf(NULL, 0, "%d", root->PID) + 2; // +2 for the parentheses
  }
  
  printPstree(child, level, showPids, numericSort);
  child = child->nextSibling;

  while (child != NULL) {
    printf("\n");
    // Print the correct number of spaces
    for (int i = 0; i < level; i++) {
      printf(" ");
    }

    printf(" ├─");
    printPstree(child, level, showPids, numericSort);
    child = child->nextSibling;
  }
}
  printf("\n");
}


// build pstree
void buildAndPrint(struct pstreeNode *root, bool showPids, bool numericSort)
{
  // 1.得到命令行的参数，根据要求设置标志变量的数值
  // 2.得到系统中所有进程的编号 (每个进程都会有唯一的编号) 保存到列表里；
  // 3.对列表里的每个编号，得到它的的父亲是谁；
  // 4.在内存中把树建好，按命令行参数要求排序；
  // 5.把树打印到终端上。

  int count = countDirectories("/proc");
  struct HashTable *hashTable = createHashTable((int)count * 1.33);
  traverse_procfs(storeInfo, hashTable);
  buildPstree(hashTable, numericSort);
  printPstree(search(hashTable, 1), 0, showPids, numericSort);
  // examinePstree(search(hashTable, 1));
  // printHashTable(hashTable);
}



int main(int argc, char *argv[]){
  // -p 或 --show-pids: 打印每个进程的进程号。
  // -n 或 --numeric-sort: 按照pid的数值从小到大顺序输出一个进程的直接孩子。
  // -V 或 --version: 打印版本信息。
  //得到命令行的参数，根据要求设置标志变量的数值
  bool showPids = false;
  bool numericSort = false;
  bool version = false;
  for (int i = 0; i < argc; i++)
  {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);

    if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--show-pids") == 0) {
      showPids = true;
    } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--numeric-sort") == 0) {
      numericSort = true;
    } else if (strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "--version") == 0) {
      version = true;
      printf("pstree (GNU coreutils) 0,1\n");
      return 0;
    }
  }
  buildAndPrint(NULL, showPids, numericSort);
  // assert(!argv[argc]);
  return 0;
}
