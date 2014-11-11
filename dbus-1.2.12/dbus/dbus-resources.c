/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/* dbus-resources.c Resource tracking/limits
 *
 * Copyright (C) 2003  Red Hat Inc.
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
#include <dbus/dbus-resources.h>
#include <dbus/dbus-internals.h>

/**
 * @defgroup DBusResources Resource limits related code
 * @ingroup  DBusInternals
 * @brief DBusCounter and other stuff related to resource limits
 *
 * Types and functions related to tracking resource limits,
 * such as the maximum amount of memory a connection can use
 * for messages, etc.
 */

/**
 * @defgroup DBusResourcesInternals Resource limits implementation details
 * @ingroup  DBusInternals
 * @brief Resource limits implementation details
 *
 * Implementation details of resource limits code.
 *
 * @{
 */

/**
 * @brief Internals of DBusCounter.
 * 
 * DBusCounter internals. DBusCounter is an opaque object, it must be
 * used via accessor functions.
 */
struct DBusCounter
{
  int refcount;  /**< reference count */

  long value;    /**< current counter value */

  long notify_guard_value; /**< call notify function when crossing this value */
  DBusCounterNotifyFunction notify_function; /**< notify function */
  void *notify_data; /**< data for notify function */
};

/** @} */  /* end of resource limits internals docs */

/**
 * @addtogroup DBusResources
 * @{
 */

/**
 * Creates a new DBusCounter. DBusCounter is used
 * to count usage of some resource such as memory.
 *
 * @returns new counter or #NULL on failure
 */
DBusCounter*
_dbus_counter_new (void)
{
  DBusCounter *counter;

  counter = dbus_new (DBusCounter, 1);
  if (counter == NULL)
    return NULL;
  
  counter->refcount = 1;
  counter->value = 0;

  counter->notify_guard_value = 0;
  counter->notify_function = NULL;
  counter->notify_data = NULL;
  
  return counter;
}

/**
 * Increments refcount of the counter
 *
 * @param counter the counter
 * @returns the counter
 */
DBusCounter *
_dbus_counter_ref (DBusCounter *counter)
{
  _dbus_assert (counter->refcount > 0);
  
  counter->refcount += 1;

  return counter;
}

/**
 * Decrements refcount of the counter and possibly
 * finalizes the counter.
 *
 * @param counter the counter
 */
void
_dbus_counter_unref (DBusCounter *counter)
{
  _dbus_assert (counter->refcount > 0);

  counter->refcount -= 1;

  if (counter->refcount == 0)
    {
      
      dbus_free (counter);
    }
}

/**
 * Adjusts the value of the counter by the given
 * delta which may be positive or negative.
 * Calls the notify function from _dbus_counter_set_notify()
 * if that function has been specified.
 *
 * @param counter the counter
 * @param delta value to add to the counter's current value
 */
void
_dbus_counter_adjust (DBusCounter *counter,
                      long         delta)
{
  long old = counter->value;
  
  counter->value += delta;

#if 0
  _dbus_verbose ("Adjusting counter %ld by %ld = %ld\n",
                 old, delta, counter->value);
#endif
  
  if (counter->notify_function != NULL &&
      ((old < counter->notify_guard_value &&
        counter->value >= counter->notify_guard_value) ||
       (old >= counter->notify_guard_value &&
        counter->value < counter->notify_guard_value)))
    (* counter->notify_function) (counter, counter->notify_data);
}

/**
 * Gets the current value of the counter.
 *
 * @param counter the counter
 * @returns its current value
 */
long
_dbus_counter_get_value (DBusCounter *counter)
{
  return counter->value;
}

/**
 * Sets the notify function for this counter; the notify function is
 * called whenever the counter's value crosses the guard value in
 * either direction (moving up, or moving down).
 *
 * @param counter the counter
 * @param guard_value the value we're notified if the counter crosses
 * @param function function to call in order to notify
 * @param user_data data to pass to the function
 */
void
_dbus_counter_set_notify (DBusCounter               *counter,
                          long                       guard_value,
                          DBusCounterNotifyFunction  function,
                          void                      *user_data)
{
  counter->notify_guard_value = guard_value;
  counter->notify_function = function;
  counter->notify_data = user_data;
}

/** @} */  /* end of resource limits exported API */
