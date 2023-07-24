/**********************************************************************
 * Copyright (C) 2017-2017  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **********************************************************************
 *
 * Description : uni_fsm.c
 * Author      : wangzhicheng@unisound.com
 * Date        : 2017.9.27
 *
 **********************************************************************/

#include "uni_fsm.h"
#include "uni_iot.h"
#include "uni_types.h"
#include "uni_log.h"

#define FSM_TAG   "fsm"

static MicroFsmTransition* _get_fsm_transition(MicroFsmStruct *fsm,
                                               uni_s32 event_id) {
  MicroFsmTransition *transition;
  uni_s32 i;
  if (NULL == fsm) {
    LOGE(FSM_TAG, "fsm is NULL.");
    return NULL;
  }
  for (i = 0; i < fsm->transition_num; i ++) {
    transition = &fsm->transition[i];
    if (transition->state == fsm->current_state &&
        transition->event_id == event_id) {
      return transition;
    }
  }
  return NULL;
}

/**
 * @brief Initialize the fsm
 * @param[in] fsm is the point to a fsm struct
 * @param[in] brief_intro is the brief description of the fsm
 * @param[in] transition is the point to a fsm transitions list
 * @param[in] transition_num is the number of the fsm transitions
 */
void FsmInit(MicroFsmStruct *fsm, char *brief_intro,
             MicroFsmTransition *transition, uni_s32 transition_num,
             State2Str state2str) {
  if (NULL == fsm || NULL == transition) {
    LOGE(FSM_TAG, "fsm or transition is NULL.");
    return;
  }
  if (strlen(brief_intro) >= sizeof(fsm->description)) {
    LOGE(FSM_TAG, "fsm description is too long.");
    return;
  }
  uni_strncpy(fsm->description, brief_intro, sizeof(fsm->description) - 1);
  fsm->current_state = -1;
  fsm->transition = transition;
  fsm->transition_num = transition_num;
  fsm->state2str = state2str;
  LOGT(FSM_TAG, "[%s]: fsm init done, transition num is %d.",
       fsm->description, fsm->transition_num);
}

/**
 * @brief Set current state
 * @param[in] fsm is the point to a fsm struct
 * @param[in] state is one of the fsm state
 */
void FsmSetState(MicroFsmStruct *fsm, uni_s32 state) {
  if (NULL == fsm) {
    LOGE(FSM_TAG, "fsm is NULL.");
    return;
  }
  LOGT(FSM_TAG, "[%s]: fsm set current state from %s to %s.",
       fsm->description, fsm->state2str(fsm->current_state),
       fsm->state2str(state));
  fsm->current_state = state;
}

/**
 * @brief Set current state
 * @param[in] fsm is the point to a fsm struct
 * @return state is one of the fsm state
 */
uni_s32 FsmGetState(MicroFsmStruct *fsm) {
  if (NULL == fsm) {
    LOGE(FSM_TAG, "fsm is NULL.");
    return -1;
  }
  LOGD(FSM_TAG, "[%s]: fsm get current state is %s.",
       fsm->description, fsm->state2str(fsm->current_state));
  return fsm->current_state;
}

/**
 * @brief Check the validity of a event
 * @param[in] fsm is the point to a fsm struct
 * @param[in] event_id is one of the event
 * @return Result
 */
Result IsValidEventId(MicroFsmStruct *fsm, uni_s32 event_id) {
  Result rc = E_REJECT;
  if (NULL == fsm) {
    LOGE(FSM_TAG, "fsm is NULL.");
    return rc;
  }
  if (NULL != _get_fsm_transition(fsm, event_id)) {
    rc = E_OK;
  }
  return rc;
}

/**
 * @brief Process a event
 * @param[in] fsm is the point to a fsm struct
 * @param[in] param is the point to the parameters which should
 *            be used in DoAction function
 * @param[in] event_id is one of the event
 * @return Result
 */
Result ProcessNextEvent(MicroFsmStruct *fsm, void *param, uni_s32 event_id) {
  Result rc = E_REJECT;
  MicroFsmTransition *transition;
  if (NULL == fsm) {
    LOGE(FSM_TAG, "fsm is NULL.");
    return rc;
  }
  LOGT(FSM_TAG, "[%s]: fsm[%s] process event[%d].",
       fsm->description, fsm->state2str(fsm->current_state), event_id);
  transition = _get_fsm_transition(fsm, event_id);
  if (transition) {
    if (transition->action) {
      rc = transition->action(param);
    }
  }
  return rc;
}
