/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/* config-parser-common.h  Common defines and routines for config file parsing
 *
 * Copyright (C) 2007 Red Hat, Inc.
 *
 * Licensed under the Academic Free License version 2.1
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef BUS_CONFIG_PARSER_COMMON_H
#define BUS_CONFIG_PARSER_COMMON_H

#include <config.h>

typedef enum
{
  ELEMENT_NONE,
  ELEMENT_BUSCONFIG,
  ELEMENT_INCLUDE,
  ELEMENT_USER,
  ELEMENT_LISTEN,
  ELEMENT_AUTH,
  ELEMENT_POLICY,
  ELEMENT_LIMIT,
  ELEMENT_ALLOW,
  ELEMENT_DENY,
  ELEMENT_FORK,
  ELEMENT_PIDFILE,
  ELEMENT_SERVICEDIR,
  ELEMENT_SERVICEHELPER,
  ELEMENT_INCLUDEDIR,
  ELEMENT_TYPE,
  ELEMENT_SELINUX,
  ELEMENT_ASSOCIATE,
  ELEMENT_STANDARD_SESSION_SERVICEDIRS,
  ELEMENT_STANDARD_SYSTEM_SERVICEDIRS,
  ELEMENT_SYSLOG,
  ELEMENT_KEEP_UMASK
} ElementType;

ElementType bus_config_parser_element_name_to_type (const char *element_name);
const char* bus_config_parser_element_type_to_name (ElementType type);

#endif /* BUS_CONFIG_PARSER_COMMON_H */

