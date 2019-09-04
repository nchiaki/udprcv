
/* 
 * �L���[�f�[�^��舵���}�N��
 *
 * 2014.10.17 at first from NORTi
 * 2012/10/20 chiaki	first release
 */

#ifndef	queue_inc
#define queue_inc

typedef struct	queue {	/* �L���[�̈��` */
	struct queue	*que_next;
	struct queue	*que_back;
} QUE;

typedef	QUE	*QLINK;

/* �L���[�̏����� */
#define	QINIT(que)	(((QLINK)(que))->que_next = (((QLINK)que))->que_back = (QLINK)(que))

/* �O�L���[�̎擾 */
#define	NEXT(que)	(((QLINK)(que))->que_next)

/* ��L���[�̎擾 */
#define	BACK(que)	(((QLINK)(que))->que_back)

/* �L���[�f�[�^�̔����o�� */
#define	QREMOVE(que)	{BACK(que)->que_next = NEXT(que); NEXT(que)->que_back = BACK(que); QINIT(que); }

/* �L���[�f�[�^�̒ǉ� */
#define	QAPPEND(pre, que)	\
	{BACK(que) = (QLINK)pre; NEXT(que) = NEXT(pre); NEXT(pre) = (QLINK)que; NEXT(que)->que_back = (QLINK)que;}

/* �L���[�f�[�^�̑}�� */
#define	QINSERT(que, aft) \
	{NEXT(que) = (QLINK)aft; BACK(que) = BACK(aft); BACK(aft) = (QLINK)que; BACK(que)->que_next = (QLINK)que;}

/* ���[�g�L���[�ւ̃G���L���[ */
#define	ENQUEUE(root, que)	\
	{BACK(que) = BACK(root); BACK(root)->que_next = (QLINK)(que); NEXT(que) = (root); BACK(root) = (QLINK)(que);}

/* ���[�g�L���[����̃f�L���[ */
//#define	DEQUEUE(que, root)	{(QLINK)(que) = NEXT(root); QREMOVE(que);}		/*gcc4�ȍ~�ō��Ӓl�̃L���X�g�֎~�̂��߃R���p�C�����ʂ�Ȃ�*/
//#define	DEQUEUE(que, root)	{(que) = NEXT(root); QREMOVE(que);}
#define	DEQUEUE(que, cast, root)	{que = cast NEXT(root); QREMOVE(que);}

/* ���[�g�L���[�ւ̃G���L���[��� */
#define	QisEMPTY(root)	((NEXT(root)==(root)) && (BACK(root)==(root)))

/* ���[�g�L���[�̘A�� */
#define	QJOIN(dstrt, srcrt)	{QUE	*mstbck, *nwtop, *nwend;\
			nwtop = (srcrt)->que_next; nwend = (srcrt)->que_back;\
			mstbck = (dstrt)->que_back;\
			mstbck->que_next = nwtop;\
			nwtop->que_back = mstbck;\
			(dstrt)->que_back = nwend;\
			nwend->que_next = (dstrt);\
			QINIT(srcrt);\
}

#endif /* queue_inc */
