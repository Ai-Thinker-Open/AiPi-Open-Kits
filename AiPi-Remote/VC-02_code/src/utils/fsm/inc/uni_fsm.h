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
 * Description : uni_fsm.h
 * Author      : wangzhicheng@unisound.com
 * Date        : 2017.9.27
 *
 **********************************************************************/

#ifndef UTILS_FSM_INC_UNI_FSM_H_
#define UTILS_FSM_INC_UNI_FSM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "uni_types.h"

typedef Result (*DoAction)(void *args);
typedef const char* (*State2Str)(uni_s32 state);

typedef struct {
  uni_s32  state;
  uni_s32  event_id;
  DoAction action;
} MicroFsmTransition;

typedef struct {
  char               description[16];
  uni_s32            current_state;
  uni_s32            transition_num;
  State2Str          state2str;
  MicroFsmTransition *transition;
} MicroFsmStruct;

/**
 * @brief Initialize the fsm
 * @param[in] fsm is the point to a fsm struct
 * @param[in] brief_intro is the brief description of the fsm
 * @param[in] transition is the point to a fsm transitions list
 * @param[in] transition_num is the number of the fsm transitions
 */
void FsmInit(MicroFsmStruct *fsm, char *brief_intro,
             MicroFsmTransition *transition, uni_s32 transition_num,
             State2Str state2str);

/**
 * @brief Set current state
 * @param[in] fsm is the point to a fsm struct
 * @param[in] state is one of the fsm state
 */
void FsmSetState(MicroFsmStruct *fsm, uni_s32 state);

/**
 * @brief Set current state
 * @param[in] fsm is the point to a fsm struct
 * @return state is one of the fsm state
 */
uni_s32 FsmGetState(MicroFsmStruct *fsm);

/**
 * @brief Check the validity of a event
 * @param[in] fsm is the point to a fsm struct
 * @param[in] event_id is one of the event
 * @return Result
 */
Result IsValidEventId(MicroFsmStruct *fsm, uni_s32 event_id);

/**
 * @brief Process a event
 * @param[in] fsm is the point to a fsm struct
 * @param[in] param is the point to the parameters which should
 *            be used in DoAction function
 * @param[in] event_id is one of the event
 * @return Result
 */
Result ProcessNextEvent(MicroFsmStruct *fsm, void *param, uni_s32 event_id);

#ifdef __cplusplus
}
#endif
#endif  // UTILS_FSM_INC_UNI_FSM_H_
