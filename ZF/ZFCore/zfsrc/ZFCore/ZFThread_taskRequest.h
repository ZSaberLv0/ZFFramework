/**
 * @file ZFThread_taskRequest.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_taskRequest_h_
#define _ZFI_ZFThread_taskRequest_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// task request
/** @brief see #ZFThreadTaskRequest */
zfclass ZF_ENV_EXPORT ZFThreadTaskRequestData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFThreadTaskRequestData, ZFObject)

public:
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_ASSIGN_READONLY(zfidentity, taskId, zfidentityInvalid())
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_ASSIGN(ZFListener, taskCallback)
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskParam0)
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskParam1)
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_RETAIN(ZFObject *, taskUserData)
    /** @brief see #ZFThreadTaskRequest */
    ZFPROPERTY_ASSIGN(ZFObject *, taskOwner)
};
/** @brief see #ZFThreadTaskRequest */
zfclass ZF_ENV_EXPORT ZFThreadTaskRequestMergeCallbackData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(ZFThreadTaskRequestMergeCallbackData, ZFObject)

protected:
    ZFThreadTaskRequestMergeCallbackData(void)
    : taskRequestDataOld(zfnull)
    , taskRequestDataNew(zfnull)
    , taskRequestDataMerged(zfnull)
    {
    }

public:
    /** @brief see #ZFThreadTaskRequest */
    ZFThreadTaskRequestData *taskRequestDataOld;
    /** @brief see #ZFThreadTaskRequest */
    ZFThreadTaskRequestData *taskRequestDataNew;
    /** @brief see #ZFThreadTaskRequest */
    ZFThreadTaskRequestData *taskRequestDataMerged;
};

// ============================================================
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask(void);
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask(void);
extern ZF_ENV_EXPORT const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge(void);
/** @cond ZFPrivateDoc */
#define ZFThreadTaskRequestMergeCallbackIgnoreOldTask() _ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask()
#define ZFThreadTaskRequestMergeCallbackIgnoreNewTask() _ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask()
#define ZFThreadTaskRequestMergeCallbackDoNotMerge() _ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge()
#define ZFThreadTaskRequestMergeCallbackDefault() ZFThreadTaskRequestMergeCallbackIgnoreOldTask()
/** @endcond */
/**
 * @brief see #ZFThreadTaskRequest, simply ignore old task and replace with new task
 */
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFListener, ZFThreadTaskRequestMergeCallbackIgnoreOldTask, ZFThreadTaskRequestMergeCallbackIgnoreOldTask())
/**
 * @brief see #ZFThreadTaskRequest, simply ignore new task and use the old one
 */
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFListener, ZFThreadTaskRequestMergeCallbackIgnoreNewTask, ZFThreadTaskRequestMergeCallbackIgnoreNewTask())
/**
 * @brief see #ZFThreadTaskRequest, don't merge and add as new task
 */
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(ZFListener, ZFThreadTaskRequestMergeCallbackDoNotMerge, ZFThreadTaskRequestMergeCallbackDoNotMerge())
/**
 * @brief see #ZFThreadTaskRequest, #ZFThreadTaskRequestMergeCallbackIgnoreOldTask by default
 */
ZFEXPORT_VAR_READONLY_ALIAS_DECLARE(ZFListener, ZFThreadTaskRequestMergeCallbackDefault, ZFThreadTaskRequestMergeCallbackIgnoreOldTask)

// ============================================================
// task request
/**
 * @brief request a task that would be run on main thread after a proper time,
 *   safe to be called in background thread
 *
 * task's param is the params passed to this method,
 * and return value is ignored\n
 * \n
 * tasks are ensured scheduled in different CPU time\n
 * \n
 * if duplicated task is found, the mergeCallback would be called\n
 * two tasks are treated as duplicated task if task callback and owner are both the same,
 * while param0/param1/userData won't be compared
 *
 * \n
 * the mergeCallback's param0 is a #ZFThreadTaskRequestMergeCallbackData
 * contains old and new task data (#ZFThreadTaskRequestData),
 * you should decide merged task and set it to #ZFThreadTaskRequestMergeCallbackData::taskRequestDataMerged,
 * whether retain existing one or allocating new one,
 * or set it to null to show the task can not be merged\n
 * for example:\n
 * to ignore old task, you may retain new one and set it to taskRequestDataMerged\n
 * to ignore new task, you may retain the old one instead\n
 * to merge, allocate a new task data and setup necessary info\n
 * to disable merge, set taskRequestDataMerged to null, and old/new task would be scheduled separately\n
 * or, you may use the pre-defined callbacks such as #ZFThreadTaskRequestMergeCallbackDoNotMerge
 */
ZFMETHOD_FUNC_DECLARE_6(zfidentity, ZFThreadTaskRequest,
                        ZFMP_IN(const ZFListener &, taskCallback),
                        ZFMP_IN_OPT(ZFObject *, taskUserData, zfnull),
                        ZFMP_IN_OPT(ZFObject *, taskParam0, zfnull),
                        ZFMP_IN_OPT(ZFObject *, taskParam1, zfnull),
                        ZFMP_IN_OPT(ZFObject *, taskOwner, zfnull),
                        ZFMP_IN_OPT(const ZFListener &, taskMergeCallback, ZFThreadTaskRequestMergeCallbackDefault()))
/** @brief see #ZFThreadTaskRequest */
ZFMETHOD_FUNC_DECLARE_2(zfidentity, ZFThreadTaskRequest,
                        ZFMP_IN(ZFThreadTaskRequestData *, taskRequestData),
                        ZFMP_IN_OPT(const ZFListener &, mergeCallback, ZFThreadTaskRequestMergeCallbackDefault()))
/**
 * @brief see #ZFThreadTaskRequest
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFThreadTaskCancel,
                        ZFMP_IN(zfidentity, taskId))
/**
 * @brief see #ZFThreadTaskRequest
 */
ZFMETHOD_FUNC_DECLARE_4(void, ZFThreadTaskCancelExactly,
                        ZFMP_IN(const ZFListener &, task),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                        ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                        ZFMP_IN_OPT(ZFObject *, param1, zfnull))
/**
 * @brief see #ZFThreadTaskRequest
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFThreadTaskCancel,
                        ZFMP_IN(const ZFListener &, task))
/**
 * @brief see #ZFThreadTaskRequest
 *
 * owner would be compared by pointer value
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFThreadTaskCancelWithOwner,
                        ZFMP_IN(ZFObject *, owner))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_taskRequest_h_

