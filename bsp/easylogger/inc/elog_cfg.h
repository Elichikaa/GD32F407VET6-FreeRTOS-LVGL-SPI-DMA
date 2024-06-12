/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015-2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: It is the configure head file for this library.
 * Created on: 2015-07-30
 */

#ifndef _ELOG_CFG_H_
#define _ELOG_CFG_H_
/*---------------------------------------------------------------------------*/
/* enable log output. */
/* 开启输出使能 */
#define ELOG_OUTPUT_ENABLE

/* setting static output log level. range: from ELOG_LVL_ASSERT to ELOG_LVL_VERBOSE */
/* 设置静态输出日志级别。范围：从ELOG_LVL_ASSERT到ELOG-LVL-VERBOSE */
#define ELOG_OUTPUT_LVL                          ELOG_LVL_VERBOSE

/* enable assert check */
/* 开启断言检测 */
#define ELOG_ASSERT_ENABLE

/* buffer size for every line's log */
/* 每行日志的缓冲区大小 */
#define ELOG_LINE_BUF_SIZE                       1024

/* output line number max length */
/* 输出行号最大长度 */
#define ELOG_LINE_NUM_MAX_LEN                    5

/* output filter's tag max length */
#define ELOG_FILTER_TAG_MAX_LEN                  30

/* output filter's keyword max length */
/* 输出过滤器的标记最大长度 */
#define ELOG_FILTER_KW_MAX_LEN                   16

/* output filter's tag level max num */
/* 输出过滤器的标记级别最大值 */
#define ELOG_FILTER_TAG_LVL_MAX_NUM              5

/* output newline sign */
/* 换行符定义 */
#define ELOG_NEWLINE_SIGN                        "\r\n"
/*---------------------------------------------------------------------------*/
/* enable log color */
/* 开启日志颜色 */
#define ELOG_COLOR_ENABLE
/* change the some level logs to not default color if you want */
/* 如果需要，请将某些级别日志更改为非默认颜色 */
#define ELOG_COLOR_ASSERT                        (F_MAGENTA B_NULL S_NORMAL)
#define ELOG_COLOR_ERROR                         (F_RED B_NULL S_NORMAL)
#define ELOG_COLOR_WARN                          (F_YELLOW B_NULL S_NORMAL)
#define ELOG_COLOR_INFO                          (F_CYAN B_NULL S_NORMAL)
#define ELOG_COLOR_DEBUG                         (F_GREEN B_NULL S_NORMAL)
#define ELOG_COLOR_VERBOSE                       (F_BLUE B_NULL S_NORMAL)
/*---------------------------------------------------------------------------*/
/* enable log fmt */
/*  */
/* comment it if you don't want to output them at all */
/*  */
#define ELOG_FMT_USING_FUNC
#define ELOG_FMT_USING_DIR
#define ELOG_FMT_USING_LINE
/*---------------------------------------------------------------------------*/
/* enable asynchronous output mode */
/* 开启异步输出模式 */
//#define ELOG_ASYNC_OUTPUT_ENABLE

/* the highest output level for async mode, other level will sync output */
/* 异步模式的最高输出级别，其他级别将同步输出 */
#define ELOG_ASYNC_OUTPUT_LVL                    ELOG_LVL_ASSERT

/* buffer size for asynchronous output mode */
/* 异步输出模式的缓冲区大小 */
#define ELOG_ASYNC_OUTPUT_BUF_SIZE               (ELOG_LINE_BUF_SIZE * 10)

/* each asynchronous output's log which must end with newline sign */
/* 每个异步输出的日志必须以换行符结尾 */
#define ELOG_ASYNC_LINE_OUTPUT

/* asynchronous output mode using POSIX pthread implementation */
/* 使用POSIX pthread实现的异步输出模式 */
#define ELOG_ASYNC_OUTPUT_USING_PTHREAD
/*---------------------------------------------------------------------------*/
/* enable buffered output mode */
/* 启用缓冲输出模式 */
//#define ELOG_BUF_OUTPUT_ENABLE

/* buffer size for buffered output mode */
/* 缓冲输出模式的缓冲区大小 ：如果是在逻辑程序上使用该方法这里就会出现输出10行串口才开始打印 */
#define ELOG_BUF_OUTPUT_BUF_SIZE                 (ELOG_LINE_BUF_SIZE * 10)

#endif /* _ELOG_CFG_H_ */
