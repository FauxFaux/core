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
/* File: cf3.extern.h                                                        */
/*                                                                           */
/* Created: Thu Aug  2 12:51:18 2007                                         */
/*                                                                           */
/*****************************************************************************/

/* See variables in cf3globals.c and syntax.c */

extern struct PromiseParser P;

extern struct Bundle *BUNDLES;
extern struct Body *BODIES;
extern struct Scope *VSCOPE;
extern struct Audit *AUDITPTR;
extern struct Audit *VAUDIT; 
extern struct Rlist *VINPUTLIST;
extern struct Rlist *BODYPARTS;
extern struct Rlist *SUBBUNDLES;
extern struct Rlist *SINGLE_COPY_LIST;
extern struct Rlist *AUTO_DEFINE_LIST;
extern struct Rlist *SINGLE_COPY_CACHE;
extern struct Rlist *ACCESSLIST;

extern struct Rlist *CF_STCK;
extern int CF_STCKFRAME;

extern int XML;
extern FILE *FOUT;
extern struct FnCallStatus FNCALL_STATUS;

extern struct SubTypeSyntax CF_NOSTYPE;
extern char *CF_DATATYPES[];
extern char *CF_AGENTTYPES[];
extern char HASHDB[CF_BUFSIZE];
extern int FSTAB_EDITS;

extern int CFA_MAXTHREADS;
extern char THIS_AGENT[CF_MAXVARSIZE];
extern enum cfagenttype THIS_AGENT_TYPE;
extern short INSTALL_SKIP;
extern short SHOWREPORTS;
extern int FACILITY;
extern time_t PROMISETIME;
extern int ABORTBUNDLE;
extern struct Item *ABORTBUNDLEHEAP;

extern struct Rlist *SERVERLIST;
extern struct Item *PROCESSTABLE;
extern struct Item *FSTABLIST;
extern struct Rlist *MOUNTEDFSLIST;

extern int CF_MOUNTALL;
extern int CF_SAVEFSTAB;

/***********************************************************/
/* SYNTAX MODULES                                          */
/***********************************************************/

#ifndef CF3_MOD_COMMON
extern struct SubTypeSyntax CF_COMMON_SUBTYPES[];
extern struct BodySyntax CF_BODY_TRANSACTION[];
extern struct BodySyntax CF_VARBODY[];
extern struct BodySyntax CF_CLASSBODY[];
extern struct BodySyntax CFG_CONTROL[];
extern struct BodySyntax CFA_CONTROL[];
extern struct BodySyntax CFS_CONTROL[];
extern struct BodySyntax CFE_CONTROL[];
extern struct BodySyntax CFR_CONTROL[];
extern struct BodySyntax CFEX_CONTROL[];
extern struct BodySyntax CF_TRIGGER_BODY[];

extern struct BodySyntax CF_TRANSACTION_BODY[];
extern struct BodySyntax CF_DEFINECLASS_BODY[];
extern struct BodySyntax CF_COMMON_BODIES[];

extern struct SubTypeSyntax *CF_ALL_SUBTYPES[];
extern struct SubTypeSyntax CF_ALL_BODIES[];
extern struct FnCallType CF_FNCALL_TYPES[];
#endif

#ifndef CF3_MOD_ACCESS
extern struct BodySyntax CF_REMACCESS_BODIES[];
extern struct SubTypeSyntax CF_REMACCESS_SUBTYPES[];
#endif

#ifndef CF_MOD_INTERFACES
extern struct BodySyntax CF_TCPIP_BODY[];
extern struct BodySyntax CF_INTERFACES_BODIES[];
extern struct SubTypeSyntax CF_INTERFACES_SUBTYPES[];
#endif

#ifndef CF3_MOD_STORAGE
extern struct BodySyntax CF_STORAGE_BODIES[];
extern struct SubTypeSyntax CF_STORAGE_SUBTYPES[];
extern struct BodySyntax CF_MOUNT_BODY[];
extern struct BodySyntax CF_CHECKVOL_BODY[];
#endif

#ifndef CF3_MOD_KNOWLEGDE
extern struct SubTypeSyntax CF_KNOWLEDGE_SUBTYPES[];
extern struct BodySyntax CF_TOPICS_BODIES[];
extern struct BodySyntax CF_OCCUR_BODIES[];
#endif

#ifndef CF3_MOD_PACKAGES
extern struct SubTypeSyntax CF_PACKAGES_SUBTYPES[];
extern struct BodySyntax CF_PACKAGES_BODIES[];
extern struct BodySyntax CF_EXISTS_BODY[];
#endif

#ifndef CF3_MOD_REPORT
extern struct SubTypeSyntax CF_REPORT_SUBTYPES[];
extern struct BodySyntax CF_REPORT_BODIES[];
#endif


#ifndef CF3_MOD_FILES
extern struct BodySyntax CF_COMMON_EDITBODIES[];
extern struct SubTypeSyntax CF_FILES_SUBTYPES[];
extern struct BodySyntax CF_APPEND_REPL_BODIES[];
extern struct BodySyntax CF_FILES_BODIES[];
extern struct BodySyntax CF_COPYFROM_BODY[];
extern struct BodySyntax CF_LINKTO_BODY[];
extern struct BodySyntax CF_FILEFILTER_BODY[];
extern struct BodySyntax CF_CHANGEMGT_BODY[];
extern struct BodySyntax CF_TIDY_BODY[];
extern struct BodySyntax CF_RENAME_BODY[];
extern struct BodySyntax CF_RECURSION_BODY[];
#endif

#ifndef CF3_MOD_EXEC
extern struct SubTypeSyntax CF_EXEC_SUBTYPES[];
#endif

#ifndef CF3_MOD_METHODS
extern struct SubTypeSyntax CF_METHOD_SUBTYPES[];
#endif

#ifndef CF3_MOD_PROCESS
extern struct SubTypeSyntax CF_PROCESS_SUBTYPES[];
extern struct BodySyntax CF_MATCHCLASS_BODY[];
extern struct BodySyntax CF_PROCFILTER_BODY[];
extern struct BodySyntax CF_PROCESS_BODIES[];
#endif
