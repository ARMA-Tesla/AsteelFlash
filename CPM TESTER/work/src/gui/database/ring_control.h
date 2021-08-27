#if !defined(__RING_CONTROL_H__)
#define __RING_CONTROL_H__

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#define RING_GROUPS_LOCK			0
#define RING_GROUPS_UNLOCK			1
#define RING_GROUPS_HIDE			2
#define RING_GROUPS_VISIBLE			3
#define RING_GROUPS_SET_PATH		4
#define RING_GROUPS_SET_INI_PATH	5
#define	RING_GROUPS_RELOAD			6

/* first ini */	
SElExceptionPtr RingGroupIni(void); 
/* product view */
SElExceptionPtr RingGroupStepSet(int32_t control, int32_t value);
/* test steps view */
SElExceptionPtr RingGroupProductSet(int32_t control, int32_t value);   
/* line process view */
SElExceptionPtr RingGroupProcessSet(int32_t control, int32_t value);   
/* set attribute */
SElExceptionPtr RingGroupAttribute(int32_t mode); 

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !defined(__RING_CONTROL_H__)
