/**************************************************************************
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
 **************************************************************************
 *
 * Description : user_start.c
 * Author      : wufangfang@unisound.com
 * Date        : 2020.07.15
 *
 **************************************************************************/
#include "unione.h"
#include "user_event.h"
#include "user_asr.h"
#include "user_player.h"

#define TAG "user_start"

static void _custom_setting_cb(USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_custom_setting_t *setting = NULL;
  if (context) {
    setting = &context->custom_setting;
    /* add your code at here if need to parse command and do something */
    user_player_reply_list_random(setting->reply_files);
  }
}

static void _goto_awakened_cb(USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_goto_awakend_t *awkened = NULL;
  if (context) {
    awkened = &context->goto_awakend;
    /* add your code at here if need to do something when wakeup */
    user_player_reply_list_random(awkened->reply_files);
  }
}

static void _goto_sleeping_cb (USER_EVENT_TYPE event,
                                     user_event_context_t *context) {
  event_goto_sleeping_t *sleeping = NULL;
  if (context) {
    sleeping = &context->goto_sleeping;
    /* add your code at here if need to do something when sleep */
    user_player_reply_list_random(sleeping->reply_files);
  }
}

static void _register_event_callback(void) {
  user_event_subscribe_event(USER_CUSTOM_SETTING, _custom_setting_cb);
  user_event_subscribe_event(USER_GOTO_AWAKENED, _goto_awakened_cb);
  user_event_subscribe_event(USER_GOTO_SLEEPING, _goto_sleeping_cb);
  /* add your code at here if you need to care more event */
}

int user_start_init(void) {
  /* add your code at here if you need to init your device */
  _register_event_callback();
  return 0;
}


