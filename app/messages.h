
#ifndef MESSAGES_H
#define MESSAGES_H

////////////////////////////////////////////////////////////////////////////////


enum {
   // to president
   MSG_CTRLtoPRES_INIT,
   MSG_CTRLtoPRES_LOOP,
   MSG_MNGRtoPRES_STATUS,
   // to manager
   MSG_CTRLtoMNGR_INIT,
   MSG_CTRLtoMNGR_LOOP,
   MSG_EMPLtoMNGR_STATUS,
   // to employee
   MSG_CTRLtoEMPL_INIT,
   MSG_CTRLtoEMPL_LOOP,
   MSG_STUDtoEMPL_STATUS,
   // to student
   MSG_CTRLtoSTUD_INIT,
   MSG_CTRLtoSTUD_LOOP,
};

////////////////////////////////////////////////////////////////////////////////

extern PRESTO_TASKID_T ctrl_tid;
extern PRESTO_TASKID_T pres_tid;
extern PRESTO_TASKID_T mngr_tid;
extern PRESTO_TASKID_T empl_tid;
extern PRESTO_TASKID_T stud_tid;
extern PRESTO_TASKID_T dbug_tid;

////////////////////////////////////////////////////////////////////////////////

#endif // MESSAGES_H

