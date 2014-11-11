/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/* dbus-print-message.h  Utility function to print out a message
 *
 * Copyright (C) 2003 Philip Blundell <philb@gnu.org>
 * Copyright (C) 2003 Red Hat, Inc.
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
#include "dbus-print-message.h"

static const char*
type_to_name (int message_type)
{
  switch (message_type)
    {
    case DBUS_MESSAGE_TYPE_SIGNAL:
      return "signal";
    case DBUS_MESSAGE_TYPE_METHOD_CALL:
      return "method call";
    case DBUS_MESSAGE_TYPE_METHOD_RETURN:
      return "method return";
    case DBUS_MESSAGE_TYPE_ERROR:
      return "error";
    default:
      return "(unknown message type)";
    }
}


static void
indent (int depth)
{
  while (depth-- > 0)
    printf ("   ");
}

static void
print_iter (DBusMessageIter *iter, dbus_bool_t literal, int depth)
{
  do
    {
      int type = dbus_message_iter_get_arg_type (iter);

      if (type == DBUS_TYPE_INVALID)
	break;
      
      indent(depth);

      switch (type)
	{
	case DBUS_TYPE_STRING:
	  {
	    char *val;
	    dbus_message_iter_get_basic (iter, &val);
	    if (!literal)
	      printf ("string \"");
	    printf ("%s", val);
	    if (!literal)
	      printf ("\"\n");
	    break;
	  }

	case DBUS_TYPE_SIGNATURE:
	  {
	    char *val;
	    dbus_message_iter_get_basic (iter, &val);
	    if (!literal)
	      printf ("signature \"");
	    printf ("%s", val);
	    if (!literal)
	      printf ("\"\n");
	    break;
	  }

	case DBUS_TYPE_OBJECT_PATH:
	  {
	    char *val;
	    dbus_message_iter_get_basic (iter, &val);
	    if (!literal)
	      printf ("object path \"");
	    printf ("%s", val);
	    if (!literal)
	      printf ("\"\n");
	    break;
	  }

	case DBUS_TYPE_INT16:
	  {
	    dbus_int16_t val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("int16 %d\n", val);
	    break;
	  }

	case DBUS_TYPE_UINT16:
	  {
	    dbus_uint16_t val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("uint16 %u\n", val);
	    break;
	  }

	case DBUS_TYPE_INT32:
	  {
	    dbus_int32_t val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("int32 %d\n", val);
	    break;
	  }

	case DBUS_TYPE_UINT32:
	  {
	    dbus_uint32_t val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("uint32 %u\n", val);
	    break;
	  }

	case DBUS_TYPE_INT64:
	  {
	    dbus_int64_t val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("int64 %lld\n", val);
	    break;
	  }

	case DBUS_TYPE_UINT64:
	  {
	    dbus_uint64_t val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("uint64 %llu\n", val);
	    break;
	  }

	case DBUS_TYPE_DOUBLE:
	  {
	    double val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("double %g\n", val);
	    break;
	  }

	case DBUS_TYPE_BYTE:
	  {
	    unsigned char val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("byte %d\n", val);
	    break;
	  }

	case DBUS_TYPE_BOOLEAN:
	  {
	    dbus_bool_t val;
	    dbus_message_iter_get_basic (iter, &val);
	    printf ("boolean %s\n", val ? "true" : "false");
	    break;
	  }

	case DBUS_TYPE_VARIANT:
	  {
	    DBusMessageIter subiter;

	    dbus_message_iter_recurse (iter, &subiter);

	    printf ("variant ");
	    print_iter (&subiter, literal, depth+1);
	    break;
	  }
	case DBUS_TYPE_ARRAY:
	  {
	    int current_type;
	    DBusMessageIter subiter;

	    dbus_message_iter_recurse (iter, &subiter);

	    printf("array [\n");
	    while ((current_type = dbus_message_iter_get_arg_type (&subiter)) != DBUS_TYPE_INVALID)
	      {
		print_iter (&subiter, literal, depth+1);
		dbus_message_iter_next (&subiter);
		if (dbus_message_iter_get_arg_type (&subiter) != DBUS_TYPE_INVALID)
		  printf (",");
	      }
	    indent(depth);
	    printf("]\n");
	    break;
	  }
	case DBUS_TYPE_DICT_ENTRY:
	  {
	    DBusMessageIter subiter;

	    dbus_message_iter_recurse (iter, &subiter);

	    printf("dict entry(\n");
	    print_iter (&subiter, literal, depth+1);
	    dbus_message_iter_next (&subiter);
	    print_iter (&subiter, literal, depth+1);
	    indent(depth);
	    printf(")\n");
	    break;
	  }
	    
	case DBUS_TYPE_STRUCT:
	  {
	    int current_type;
	    DBusMessageIter subiter;

	    dbus_message_iter_recurse (iter, &subiter);

	    printf("struct {\n");
	    while ((current_type = dbus_message_iter_get_arg_type (&subiter)) != DBUS_TYPE_INVALID)
	      {
		print_iter (&subiter, literal, depth+1);
		dbus_message_iter_next (&subiter);
		if (dbus_message_iter_get_arg_type (&subiter) != DBUS_TYPE_INVALID)
		  printf (",");
	      }
	    indent(depth);
	    printf("}\n");
	    break;
	  }
	    
	default:
	  printf (" (dbus-monitor too dumb to decipher arg type '%c')\n", type);
	  break;
	}
    } while (dbus_message_iter_next (iter));
}

void
print_message (DBusMessage *message, dbus_bool_t literal)
{
  DBusMessageIter iter;
  const char *sender;
  const char *destination;
  int message_type;

  message_type = dbus_message_get_type (message);
  sender = dbus_message_get_sender (message);
  destination = dbus_message_get_destination (message);
  
  if (!literal)
    {
      printf ("%s sender=%s -> dest=%s",
	      type_to_name (message_type),
	      sender ? sender : "(null sender)",
	      destination ? destination : "(null destination)");
  
      switch (message_type)
	{
	case DBUS_MESSAGE_TYPE_METHOD_CALL:
	case DBUS_MESSAGE_TYPE_SIGNAL:
	  printf (" serial=%u path=%s; interface=%s; member=%s\n",
                  dbus_message_get_serial (message),
		  dbus_message_get_path (message),
		  dbus_message_get_interface (message),
		  dbus_message_get_member (message));
	  break;
      
	case DBUS_MESSAGE_TYPE_METHOD_RETURN:
	  printf (" reply_serial=%u\n",
          dbus_message_get_reply_serial (message));
	  break;

	case DBUS_MESSAGE_TYPE_ERROR:
	  printf (" error_name=%s reply_serial=%u\n",
		  dbus_message_get_error_name (message),
          dbus_message_get_reply_serial (message));
	  break;

	default:
	  printf ("\n");
	  break;
	}
    }

  dbus_message_iter_init (message, &iter);
  print_iter (&iter, literal, 1);
  fflush (stdout);
  
}

