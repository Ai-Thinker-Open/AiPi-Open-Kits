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
 * Description : uni_databuf.c
 * Author      : shangjinlong.unisound.com
 * Date        : 2018.06.19
 *
 **************************************************************************/
#include "uni_databuf.h"
#include "uni_iot.h"
#include "uni_log.h"

#define DATABUF_FILL_BYTE_SIZE   (1)
#define DATA_BUF_ATTR_READ       (1 << 0)
#define DATA_BUF_ATTR_SYNC       (1 << 1)
#define DATA_BUF_ATTR_WRITE      (1 << 2)

#define DATABUF_TAG "data_buf"

typedef int DataBufferAttr;

typedef struct {
  int  size;
  char *buf;
  int  head, tail;
} DataBuf;

static DataBuf *_create_data_buf(void) {
  DataBuf *data_buf = (DataBuf *)uni_malloc(sizeof(DataBuf));
  if (NULL == data_buf) {
    LOGE(DATABUF_TAG, "malloc failed !");
    return NULL;
  }
  data_buf->head = data_buf->tail = 0;
  return data_buf;
}

static int _get_data_size(DataBuf *data_buf) {
  return (data_buf->head + data_buf->size - data_buf->tail) % data_buf->size;
}

static int _get_free_size(DataBuf *data_buf) {
  return data_buf->size - _get_data_size(data_buf) - DATABUF_FILL_BYTE_SIZE;
}

static int _init_data_buf(void *data_buf, int size) {
  DataBuf *buf = (DataBuf *)data_buf;
  buf->size = size + DATABUF_FILL_BYTE_SIZE;
  buf->buf = (char *)uni_malloc(size + DATABUF_FILL_BYTE_SIZE);
  if (NULL == buf->buf) {
    LOGE(DATABUF_TAG, "malloc failed !");
    return -1;
  }
  buf->head = buf->tail = 0;
  return 0;
}

static inline int _read_attr_setted(DataBufferAttr attr) {
  return ((attr & DATA_BUF_ATTR_READ) == 0 ? 0 : 1);
}

static inline int _write_attr_setted(DataBufferAttr attr) {
  return ((attr & DATA_BUF_ATTR_WRITE) == 0 ? 0 : 1);
}

static inline int _sync_attr_setted(DataBufferAttr attr) {
  return ((attr & DATA_BUF_ATTR_SYNC) == 0 ? 0 : 1);
}

static inline int _read_length_overflow(DataBuf *data_buf, int readlen) {
  return _get_data_size(data_buf) < readlen;
}

static inline int _write_length_overflow(DataBuf *data_buf, int writelen) {
  return _get_free_size(data_buf) < writelen;
}

static int _read_internal(DataBufHandle handle, char *buf, int size,
                          DataBufferAttr attr) {
  DataBuf *data_buf = (DataBuf *)handle;
  int pos;
  pos = (data_buf->tail + size) % data_buf->size;
  if (_read_attr_setted(attr)) {
    if (pos >= data_buf->tail) {
      uni_memcpy(buf, data_buf->buf + data_buf->tail, size);
    } else {
      int remain = (data_buf->size - data_buf->tail);
      uni_memcpy(buf, data_buf->buf + data_buf->tail, remain);
      uni_memcpy(buf + remain, data_buf->buf, size - remain);
    }
  }
  if (_sync_attr_setted(attr)) {
    data_buf->tail = pos;
  }
  return size;
}

static int _write_internal(DataBufHandle handle, char *buf, int size,
                           DataBufferAttr attr) {
  DataBuf *data_buf = (DataBuf *)handle;
  int pos;
  pos = (data_buf->head + size) % data_buf->size;
  if (_write_attr_setted(attr)) {
    if (pos >= data_buf->head) {
      uni_memcpy(data_buf->buf + data_buf->head, buf, size);
    } else {
      int remain = (data_buf->size - data_buf->head);
      uni_memcpy(data_buf->buf + data_buf->head, buf, remain);
      uni_memcpy(data_buf->buf, buf + remain, size - remain);
    }
  }
  if (_sync_attr_setted(attr)) {
    data_buf->head = pos;
  }
  return size;
}

DataBufHandle DataBufferCreate(int size) {
  DataBuf *data_buf;
  if (size <= 0) {
    LOGE(DATABUF_TAG, "invalid input size = %d", size);
    return (DataBufHandle)NULL;
  }
  data_buf = _create_data_buf();
  if (0 != _init_data_buf(data_buf, size)) {
    uni_free(data_buf);
    return NULL;
  }
  return (DataBufHandle)data_buf;
}

int DataBufferDestroy(DataBufHandle handle) {
  DataBuf *data_buf = (DataBuf *)handle;
  uni_free(data_buf->buf);
  uni_free(data_buf);
  return 0;
}

static int _data_buffer_read(char *dst, int readlen, DataBufHandle handle,
                             DataBufferAttr attr) {
  if (_read_length_overflow((DataBuf *)handle, readlen)) {
    return -1;
  }
  return _read_internal(handle, dst, readlen, attr);
}

static int _data_buffer_write(DataBufHandle handle, char *src, int writelen,
                              DataBufferAttr attr) {
  if (_write_length_overflow((DataBuf *)handle, writelen)) {
    return -1;
  }
  return _write_internal(handle, src, writelen, attr);
}

int DataBufferClear(DataBufHandle handle) {
  DataBuf *data_buf = (DataBuf *)handle;
  data_buf->head = data_buf->tail = 0;
  return 0;
}

int DataBufferGetFreeSize(DataBufHandle handle) {
  DataBuf *data_buf = (DataBuf *)handle;
  return _get_free_size(data_buf);
}

int DataBufferGetDataSize(DataBufHandle handle) {
  DataBuf *data_buf = (DataBuf *)handle;
  return _get_data_size(data_buf);
}

int DataBufferPeek(char *dst, int readlen, DataBufHandle handle) {
  return _data_buffer_read(dst, readlen, handle, DATA_BUF_ATTR_READ);
}

int DataBufferWrite(DataBufHandle handle, char *src, int writelen) {
  return _data_buffer_write(handle, src, writelen,
                            DATA_BUF_ATTR_WRITE | DATA_BUF_ATTR_SYNC);
}

int DataBufferRead(char *dst, int readlen, DataBufHandle handle) {
  return _data_buffer_read(dst, readlen, handle,
                           DATA_BUF_ATTR_READ | DATA_BUF_ATTR_SYNC);
}
