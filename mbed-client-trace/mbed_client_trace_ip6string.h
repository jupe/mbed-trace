/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_TRACE_IP6STRING_H
#define MBED_TRACE_IP6STRING_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Print binary IPv6 address to a string.
 * String must contain enough room for full address, 40 bytes exact.
 * IPv4 tunneling addresses are not covered.
 * \param ip6addr IPv6 address.
 * \p buffer to write string to.
 */
void mbed_client_trace_ip6tos(const void *ip6addr, char *p);


#ifdef __cplusplus
}
#endif
#endif
