/* 
   Copyright (C) 2008 - Mark Burgess

   This file is part of Cfengine 3 - written and maintained by Mark Burgess.
 
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version. 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

*/

/*****************************************************************************/
/*                                                                           */
/* File: logging.c                                                           */
/*                                                                           */
/*****************************************************************************/

#include "cf3.defs.h"
#include "cf3.extern.h"
#include "cf3.server.h"

/*****************************************************************************/

void BeginAudit()

{ DB_ENV *dbenv = NULL;
  char name[CF_BUFSIZE];
  struct Promise dummyp;
  struct Attributes dummyattr;

memset(&dummyp,0,sizeof(dummyp));
memset(&dummyattr,0,sizeof(dummyattr));
dummyattr.transaction.audit = true;

snprintf(name,CF_BUFSIZE-1,"%s/%s",CFWORKDIR,CF_AUDITDB_FILE);

if ((errno = db_create(&AUDITDBP,dbenv,0)) != 0)
   {
   CfOut(cf_error,"db_open","Couldn't initialize audit database %s\n",name);
   return;
   }

#ifdef CF_OLD_DB
if ((errno = (AUDITDBP->open)(AUDITDBP,name,NULL,DB_BTREE,DB_CREATE,0644)) != 0)
#else
if ((errno = (AUDITDBP->open)(AUDITDBP,NULL,name,NULL,DB_BTREE,DB_CREATE,0644)) != 0)
#endif
   {
   CfOut(cf_error,"db_open","Couldn't open auditing database %s\n",name);
   return;
   }

ClassAuditLog(&dummyp,dummyattr,"Cfagent starting",CF_NOP);
}

/*****************************************************************************/

void EndAudit()

{ double total;
  char *sp,rettype;
  void *retval;
  struct Promise dummyp;
  struct Attributes dummyattr;

memset(&dummyp,0,sizeof(dummyp));
memset(&dummyattr,0,sizeof(dummyattr));
dummyattr.transaction.audit = true;

total = (double)(PR_KEPT+PR_NOTKEPT+PR_REPAIRED)/100.0;

if (GetVariable("control_common","version",&retval,&rettype) != cf_notype)
   {
   sp = (char *)retval;
   }
else
   {
   sp = "(not specified)";
   }

if (total == 0)
   {
   CfOut(cf_verbose,"","Outcome of version %s: No checks were scheduled\n",sp);
   return;
   }
else
   {   
   CfOut(cf_verbose,"","Outcome of version %s: Promises observed to be kept %.0f%%, Promises repaired %.0f%%, Promises not repaired %.0f\%\n",
            sp,
            (double)PR_KEPT/total,
            (double)PR_REPAIRED/total,
            (double)PR_NOTKEPT/total);
   }

ClassAuditLog(&dummyp,dummyattr,OUTPUT,CF_REPORT);
ClassAuditLog(&dummyp,dummyattr,"Cfagent closing",CF_NOP);


if (AUDITDBP)
   {
   AUDITDBP->close(AUDITDBP,0);
   }
}

/*****************************************************************************/

void ClassAuditLog(struct Promise *pp,struct Attributes attr,char *str,char status)

{ time_t now = time(NULL);
  char date[CF_BUFSIZE],lock[CF_BUFSIZE],key[CF_BUFSIZE],operator[CF_BUFSIZE];
  struct AuditLog newaudit;
  struct Audit *ap = pp->audit;
  struct timespec t;
  double keyval;
  int lineno = pp->lineno;

Debug("ClassAuditLog(%s)\n",str);

switch(status)
   {
   case CF_CHG:
       PR_REPAIRED++;
       AddAllClasses(attr.classes.change,attr.classes.persist,attr.classes.timer);
       break;
       
   case CF_WARN:
       PR_NOTKEPT++;
       break;
       
   case CF_TIMEX:
       PR_NOTKEPT++;
       AddAllClasses(attr.classes.timeout,attr.classes.persist,attr.classes.timer);
       break;

   case CF_FAIL:
       PR_NOTKEPT++;
       AddAllClasses(attr.classes.failure,attr.classes.persist,attr.classes.timer);
       break;
       
   case CF_DENIED:
       PR_NOTKEPT++;
       AddAllClasses(attr.classes.denied,attr.classes.persist,attr.classes.timer);
       break;
       
   case CF_INTERPT:
       PR_NOTKEPT++;
       AddAllClasses(attr.classes.interrupt,attr.classes.persist,attr.classes.timer);
       break;

   case CF_REGULAR:
       AddAllClasses(attr.classes.change,attr.classes.persist,attr.classes.timer);
       PR_REPAIRED++;
       break;
       
   case CF_UNKNOWN:
   case CF_NOP:
       AddAllClasses(attr.classes.kept,attr.classes.persist,attr.classes.timer);
       PR_KEPT++;
       break;
   }

if (AUDITDBP == NULL)
   {
   return;
   }

snprintf(date,CF_BUFSIZE,"%s",ctime(&now));
Chop(date);

ExtractOperationLock(lock);
snprintf(operator,CF_BUFSIZE-1,"[%s] op %s",date,lock);
strncpy(newaudit.operator,operator,CF_AUDIT_COMMENT-1);

if (clock_gettime(CLOCK_REALTIME,&t) == -1)
   {
   CfOut(cf_verbose,"clock_gettime","Clock gettime failure during audit transaction");
   return;
   }

keyval = (double)(t.tv_sec)+(double)(t.tv_nsec)/(double)CF_BILLION;
      
snprintf(key,CF_BUFSIZE-1,"%lf",keyval);

if (DEBUG)
   {
   AuditStatusMessage(status);
   }

if (ap != NULL)
   {
   strncpy(newaudit.comment,str,CF_AUDIT_COMMENT-1);
   strncpy(newaudit.filename,ap->filename,CF_AUDIT_COMMENT-1);
   
   if (ap->version == NULL || strlen(ap->version) == 0)
      {
      Debug("Promised in %s bundle %s (unamed version last edited at %s) at/before line %d\n",ap->filename,pp->bundle,ap->date,lineno);
      newaudit.version[0] = '\0';
      }
   else
      {
      Debug("Promised in %s bundle %s (version %s last edited at %s) at/before line %d\n",ap->filename,pp->bundle,ap->version,ap->date,lineno);
      strncpy(newaudit.version,ap->version,CF_AUDIT_VERSION-1);
      }
   
   strncpy(newaudit.date,ap->date,CF_AUDIT_DATE);
   newaudit.lineno = lineno;
   }
else
   {
   strcpy(newaudit.date,date);
   strcpy(newaudit.comment,str);
   strcpy(newaudit.filename,"schedule");
   strcpy(newaudit.version,"");
   newaudit.lineno = 0;
   }

newaudit.status = status;

if (AUDITDBP && attr.transaction.audit)
   {
   WriteDB(AUDITDBP,key,&newaudit,sizeof(newaudit));
   }
}

/*****************************************************************************/

void AddAllClasses(struct Rlist *list,int persist,enum statepolicy policy)

{ struct Rlist *rp;

if (list == NULL)
   {
   return;
   }

for (rp = list; rp != NULL; rp=rp->next)
   {
   if (!CheckParseClass("class addition",(char *)rp->item,CF_IDRANGE))
      {
      return;
      }
   
   if (IsHardClass((char *)rp->item))
      {
      CfOut(cf_error,"","You cannot use reserved hard classes as post-condition classes");
      }

   if (persist > 0)
      {
      Verbose(" ?> defining persistent class %s\n",(char *)rp->item);
      NewPersistentContext(rp->item,persist,policy);
      PrependItem(&VHEAP,CanonifyName((char *)rp->item),NULL);
      }
   else
      {
      Verbose(" ?> defining class %s\n",(char *)rp->item);
      PrependItem(&VHEAP,CanonifyName((char *)rp->item),NULL);
      }
   }
}



/************************************************************************/

void ExtractOperationLock(char *op)

{ char *sp, lastch = 'x'; 
  int i = 0, dots = 0;
  int offset = strlen("lock...")+strlen(VUQNAME);

/* Use the global copy of the lock from the main serial thread */
  
for (sp = CFLOCK+offset; *sp != '\0'; sp++)
   {
   switch (*sp)
      {
      case '_':
          if (lastch == '_')
             {
             break;
             }
          else
             {
             op[i] = '/';
             }
          break;

      case '.':
          dots++;
          op[i] = *sp;
          break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
          dots = 9;
          break;
          
      default:
          op[i] = *sp;
          break;
      }

   lastch = *sp;
   i++;
   
   if (dots > 1)
      {
      break;
      }
   }

op[i] = '\0';
}
