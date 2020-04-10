#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "cachelab.h"
#include "getopt.h"
#include "unistd.h"
#include "stdbool.h"
#include "list.h"


int main(int argc, char * argv[])
{
  int hits = 0;
  int misses = 0;
  int evictions = 0;
  int dbe = 0; //dirty_bytes_evicted
  int dba = 0; //dirty_bytes_active
  int dr = 0; //double_references

  char type = 'L';
  unsigned int address = 10;
  int nset;
  int ntag;

  int s;
  int ee;
  int b;
  int ss;
  int bb;

  char* file;
  FILE* fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  int opt;
  
  while((opt=getopt(argc, argv, "vs:E:b:t:"))!=-1){
    switch(opt){
      case 'v':
        break; 
      case 's':
        s = (int) strtol(optarg, (char **)NULL, 10);
        break;
      case 'E':
        ee = (int) strtol(optarg, (char **)NULL, 10);
        break;
      case 'b':
        b = (int) strtol(optarg, (char **)NULL, 10);
        break;
      case 't':
        file = optarg;
        break;
    }
  } 
  ss = 1<<s;
  bb = 1<<b;
  Set *sets = malloc(ss*sizeof(*sets));
  for(int i=0; i<ss; ++i){
    sets[i].max = ee;
    list_init(&sets[i].lines);
  }

  fp = fopen(file, "r");
  if(fp==NULL)
    exit(1);

  while((read=getline(&line, &len, fp))!=-1){
    sscanf(line, " %c %x,", &type, &address);
    printf("%s", line);
    ntag = address>>(s+b);
    nset = (address<<(32-s-b))>>(32-s);
    switch(type){
      case 'L':
        if(tagmatch(ntag, &sets[nset])){
          if(doubleref(ntag, &sets[nset])){
            ++dr;
          }
          ++hits;
          renew(ntag,&sets[nset],true);
        }
        else if(load(ntag,false,&sets[nset])){
          ++misses;
        }
        else{
          if(evict(&sets[nset])){//if evict returns true meaning dirty byte evicted
            ++dbe;
          }
          load(ntag,false,&sets[nset]);
          ++evictions;
          ++misses;
        }
        break;

      case 'S':
        if(tagmatch(ntag, &sets[nset])){
          if(doubleref(ntag, &sets[nset])){
            ++dr;
          }
          ++hits;
          renew(ntag,&sets[nset],false);
        }
        else if(load(ntag,true,&sets[nset])){
          ++misses;
        }
        else{
          if(evict(&sets[nset])){
            ++dbe;
          }
          load(ntag,true,&sets[nset]);
          ++evictions;
          ++misses;
        }
        break;
      case 'M':
        
        if(tagmatch(ntag, &sets[nset])){
          if(doubleref(ntag, &sets[nset])){
            ++dr;
          }
          ++hits;
          ++hits;
          ++dr;
          renew(ntag,&sets[nset],false);
        }
        else if(load(ntag,true,&sets[nset])){
          ++misses;
          ++hits;
          ++dr;
        }
        else{
          if(evict(&sets[nset])){
            ++dbe;
          }
          load(ntag,true,&sets[nset]);
          ++evictions;
          ++misses; 
          ++hits;
          ++dr;
        } 
        break;
      default:
        break;
    }
  }

  fclose(fp);
  if(line){
    free(line);
  }
  
  for(int i=0; i<ss; ++i){
    while(sets[i].lines.head!=NULL){
      if(sets[i].lines.head->val.dirty)
        ++dba;
      list_delete(&sets[i].lines, sets[i].lines.head->val);
    }
  }
  free(sets);

  printSummary(hits, misses, evictions, dbe*bb, dba*bb, dr);
  return 0;
}
