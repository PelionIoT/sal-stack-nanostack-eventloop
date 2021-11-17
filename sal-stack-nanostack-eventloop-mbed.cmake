#################################################################################
## Copyright 2020-2021 Pelion.
##
## SPDX-License-Identifier: Apache-2.0
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
#################################################################################

target_include_directories(mbed-nanostack-sal_stack-event_loop
    INTERFACE
        .
        ./nanostack-event-loop
        ./nanostack-event-loop/platform
        ./source
)

target_sources(mbed-nanostack-sal_stack-event_loop
    INTERFACE
        source/event.c
        source/nanostack_eventloop.c
        source/ns_timeout.c
        source/ns_timer.c
        source/system_timer.c
)
