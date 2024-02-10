#ifndef DEERTOS_H
#define DEERTOS_H

void OsInitThreadStack(); 
__attribute__((naked)) void LaunchScheduler();

#endif
