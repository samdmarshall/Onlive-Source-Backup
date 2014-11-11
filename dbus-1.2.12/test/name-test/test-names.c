#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <dbus/dbus-connection-internal.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#define REMOVE_CONNECTION 0
#define ADD_CONNECTION 1
#define ALLOW_REPLACEMENT DBUS_NAME_FLAG_ALLOW_REPLACEMENT
#define REPLACE_EXISTING DBUS_NAME_FLAG_REPLACE_EXISTING
#define DO_NOT_QUEUE DBUS_NAME_FLAG_DO_NOT_QUEUE

#define PRIMARY_OWNER DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER
#define IN_QUEUE DBUS_REQUEST_NAME_REPLY_IN_QUEUE
#define EXISTS DBUS_REQUEST_NAME_REPLY_EXISTS
#define ALREADY_OWNER DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER

#define RELEASED DBUS_RELEASE_NAME_REPLY_RELEASED
#define NON_EXISTANT DBUS_RELEASE_NAME_REPLY_NON_EXISTENT
#define NOT_OWNER DBUS_RELEASE_NAME_REPLY_NOT_OWNER

#define NUM_CONN 4
#define TEST_NAME "org.freedesktop.DBus.TestSuite.NameTest"
#define NUM_TRIES_TIL_FAIL 15

typedef struct {
  int command;

  int connection_number;
  dbus_uint32_t flags;

  dbus_uint32_t expected_result;

  int expected_queue[NUM_CONN];
} CommandAndResult;

CommandAndResult test_data[] = {
  {ADD_CONNECTION, 0, ALLOW_REPLACEMENT | REPLACE_EXISTING,
   PRIMARY_OWNER, {0,-1,-1,-1}},
  {ADD_CONNECTION, 0, REPLACE_EXISTING,
   ALREADY_OWNER, {0,-1,-1,-1}},
  {ADD_CONNECTION, 1, ALLOW_REPLACEMENT | REPLACE_EXISTING,
   IN_QUEUE, {0,1,-1,-1}},
  {REMOVE_CONNECTION, 0, 0,
   RELEASED, {1,-1,-1,-1}},
  {ADD_CONNECTION, 0, REPLACE_EXISTING | DO_NOT_QUEUE,
   PRIMARY_OWNER, {0,1,-1,-1}},
  {ADD_CONNECTION, 2, ALLOW_REPLACEMENT,
   IN_QUEUE, {0,1,2,-1}},
  {ADD_CONNECTION, 2, ALLOW_REPLACEMENT | REPLACE_EXISTING,
   IN_QUEUE, {0,2,1,-1}},
  {ADD_CONNECTION, 0, ALLOW_REPLACEMENT | DO_NOT_QUEUE,
   ALREADY_OWNER, {0,2,1,-1}},
  {ADD_CONNECTION, 1, ALLOW_REPLACEMENT | REPLACE_EXISTING,
   PRIMARY_OWNER, {1,2,-1,-1}},
  {ADD_CONNECTION, 0, REPLACE_EXISTING,
   PRIMARY_OWNER, {0,1,2,-1}},
  {ADD_CONNECTION, 2, DO_NOT_QUEUE,
   EXISTS, {0,1,-1,-1}},
  {REMOVE_CONNECTION, 2, 0,
   NOT_OWNER, {0,1,-1,-1}},
  {ADD_CONNECTION, 3, 0,
   IN_QUEUE, {0,1,3,-1}},
  {ADD_CONNECTION, 0, ALLOW_REPLACEMENT,
   ALREADY_OWNER, {0,1,3,-1}},
  {ADD_CONNECTION, 2, ALLOW_REPLACEMENT,
   IN_QUEUE, {0,1,3,2}}
};

static dbus_bool_t
check_connection (DBusConnection *conn,
                  int iteration,
                  DBusConnection *uniq_conn[NUM_CONN])
{
  DBusMessage *reply;
  DBusMessage *method;
  DBusError error;
  char **list;
  int len, i;
  const char *name;

  reply = NULL;
  method = NULL;
  list = NULL;

  dbus_error_init (&error);

  name = TEST_NAME;
  method = dbus_message_new_method_call (DBUS_SERVICE_DBUS,
                                         DBUS_PATH_DBUS,
                                         DBUS_INTERFACE_DBUS,
                                         "ListQueuedOwners");

  if (method == NULL)
    goto out;

  if (!dbus_message_append_args (method,
                                 DBUS_TYPE_STRING, &name,
                                 DBUS_TYPE_INVALID))
    {
      fprintf (stderr, "Error appending args\n") ;
      goto out;
    }

  reply = dbus_connection_send_with_reply_and_block (conn,
                                                     method,
                                                     -1,
                                                     &error);

  if (reply == NULL)
    {
      fprintf (stderr, "Error calling ListQueuedOwners: %s\n", error.message);
      dbus_error_free (&error);
      goto out;
    }



  if (!dbus_message_get_args (reply,
                              &error,
                              DBUS_TYPE_ARRAY, DBUS_TYPE_STRING,
                              &list, &len,
                              DBUS_TYPE_INVALID))
    {
      fprintf (stderr, "Error getting args: %s\n", error.message);
      dbus_error_free (&error);
      goto out;
    }

  printf ("Iteration %i: ", iteration);

  if (len > NUM_CONN)
    {
      fprintf (stderr, "There are %i connections in the queue,"
               " we are only expecting up to %i connections!\n",
               len,
               NUM_CONN);
      goto out;
    }

  for (i = 0; i < len; i++)
    {
      int expected_conn_num;
      const char *expected_uname;

      if (i > 0)
        printf (", ");

      printf ("%s", list[i]);

      expected_conn_num = test_data[iteration].expected_queue[i];

      if (expected_conn_num == -1)
        {
          fprintf (stderr,
                   "\nDid not expect this last connection"
                   " to be in the queue!\n");
          goto out;
        }

      expected_uname =
        dbus_bus_get_unique_name (uniq_conn[expected_conn_num]);

      if (strcmp (list[i], expected_uname) != 0)
        {
          fprintf (stderr,
                   "\n%s expected but %s is in the queue!\n",
                   expected_uname,
                   list[i]);

          goto out;
        }
    }

  printf ("\n");

  dbus_message_unref (method);
  dbus_message_unref (reply);
  dbus_free_string_array (list);
  return TRUE;

 out:
  if (method != NULL)
    dbus_message_unref (method);

  if (reply != NULL)
    dbus_message_unref (reply);

  if (list != NULL)
    dbus_free_string_array (list);

  return FALSE;
}

static dbus_bool_t
match_acquired_or_lost_signal (DBusConnection *conn, const char *member, const char *name)
{
  int tries;
  DBusMessage *msg;
  const char *interface = "org.freedesktop.DBus";

  for (tries = 0; tries < NUM_TRIES_TIL_FAIL; tries++)
    {
      _dbus_connection_lock (conn);
      _dbus_connection_do_iteration_unlocked (conn,
                                              DBUS_ITERATION_DO_READING |
                                              DBUS_ITERATION_DO_WRITING |
                                              DBUS_ITERATION_BLOCK,
                                              0);
      _dbus_connection_unlock (conn);
      msg = dbus_connection_pop_message (conn);
      if (msg != NULL)
        {
          if (dbus_message_is_signal (msg,
                                      interface,
                                      member))
            {
              const char *n;
              DBusError error;
              dbus_error_init (&error);

              dbus_message_get_args (msg, &error, DBUS_TYPE_STRING, &n, DBUS_TYPE_INVALID);

              if (dbus_error_is_set (&error))
                {
                  fprintf (stderr, "Error getting args: %s\n", error.message);
                  dbus_error_free (&error);
                  dbus_message_unref (msg);
                  return FALSE;
                }

              if (strcmp (n, name) == 0)
                {
                  dbus_message_unref (msg);
                  break;
                }
            }
          dbus_message_unref (msg);
        }
    }

  if (tries == NUM_TRIES_TIL_FAIL)
    {
      fprintf (stderr, "Did not receive the expected %s.%s signal!!!\n", interface, member);
      return FALSE;
    }

  return TRUE;
}

static dbus_bool_t
match_name_owner_changed_signal (DBusConnection *conn,
                                 const char     *bus_name,
                                 const char     *lost_name,
                                 const char     *acquired_name)
{
  int tries;
  DBusMessage *msg;

  for (tries = 0; tries < NUM_TRIES_TIL_FAIL; tries++)
    {
      _dbus_connection_lock (conn);
      _dbus_connection_do_iteration_unlocked (conn,
                                              DBUS_ITERATION_DO_READING |
                                              DBUS_ITERATION_DO_WRITING |
                                              DBUS_ITERATION_BLOCK,
                                              0);
      _dbus_connection_unlock (conn);
      msg = dbus_connection_pop_message (conn);

      if (msg != NULL)
        {
          if (dbus_message_is_signal (msg,
                                      "org.freedesktop.DBus",
                                      "NameOwnerChanged"))
            {
              const char *n;
              const char *ln;
              const char *an;
              DBusError error;
              dbus_error_init (&error);

              dbus_message_get_args (msg, &error, DBUS_TYPE_STRING, &n, DBUS_TYPE_STRING, &ln, DBUS_TYPE_STRING, &an, DBUS_TYPE_INVALID);

              if (dbus_error_is_set (&error))
                {
                  fprintf (stderr, "Error getting args: %s\n", error.message);
                  dbus_error_free (&error);
                  dbus_message_unref (msg);
                  return FALSE;
                }

              if (strcmp (n, bus_name) == 0)
                {
                  if ((lost_name == NULL && strcmp (ln, "") == 0)
                      || strcmp (lost_name, ln) == 0)
                    {
                      if ((acquired_name == NULL && strcmp (an, "") == 0)
                          || strcmp (acquired_name, an) == 0)
                        {
                          dbus_message_unref (msg);
                          break;
                        }
                      else
                        {
                          fprintf (stderr, "Error: name %s was expected to be acquired but we got %s instead\n", acquired_name, an);
                          dbus_message_unref (msg);
                          return FALSE;
                        }
                    }
                  else
                    {
                      fprintf (stderr, "Error: name %s was expected to be lost but we got %s instead\n", lost_name, ln);
                      dbus_message_unref (msg);
                      return FALSE;
                    }
                }
            }
          dbus_message_unref (msg);
        }
    }

  if (tries == NUM_TRIES_TIL_FAIL)
    {
      fprintf (stderr, "Did not receive the expected NameOwnerChanged signal, bus_name %s lost_name %s acquired_name %s\n",
               bus_name ? bus_name : "(null)",
               lost_name ? lost_name : "(null)",
               acquired_name ? acquired_name : "(null)");
      return FALSE;
    }

  return TRUE;
}


static dbus_bool_t
check_signals (DBusConnection *monitor,
               int iteration,
               DBusConnection *conn[NUM_CONN])
{
  DBusConnection *lost_conn = NULL;
  DBusConnection *acquired_conn = NULL;
  const char *lost_name;
  const char *acquired_name;

  if (iteration == 0)
    {
      int i;
      i = test_data[iteration].expected_queue[0];

      if (i >= 0)
        acquired_conn = conn[i];
    }
  else
    {
      int i;
      i = test_data[iteration - 1].expected_queue[0];

      if (i >= 0)
        lost_conn = conn[i];

      i = test_data[iteration].expected_queue[0];

      if (i >= 0)
        acquired_conn = conn[i];

      if (acquired_conn == lost_conn)
        acquired_conn = lost_conn = NULL;
    }

  lost_name = lost_conn == NULL? NULL :
    dbus_bus_get_unique_name (lost_conn);

  acquired_name = acquired_conn == NULL? NULL :
    dbus_bus_get_unique_name (acquired_conn);

  if (lost_name != NULL)
    if (!match_acquired_or_lost_signal (lost_conn,
                                        "NameLost",
                                        TEST_NAME))
      return FALSE;

  if (acquired_name != NULL)
    if (!match_acquired_or_lost_signal (acquired_conn,
                                        "NameAcquired",
                                        TEST_NAME))
      return FALSE;

  if (acquired_name != NULL || lost_name != NULL)
    if (!match_name_owner_changed_signal (monitor,
                                          TEST_NAME,
                                          lost_name,
                                          acquired_name))
      return FALSE;

  return TRUE;
}

int
main (int argc, char *argv[])
{
  DBusConnection *conn[NUM_CONN];
  DBusConnection *monitor;
  DBusError error;
  int i;
  int test_data_len;

  test_data_len = sizeof (test_data) / sizeof (CommandAndResult);

  dbus_error_init (&error);

  conn[0] = dbus_bus_get_private (DBUS_BUS_SESSION, &error);
  if (dbus_error_is_set (&error))
    {
      fprintf (stderr, "*** Failed to open connection 0 to session bus: %s\n",
               error.message);
      dbus_error_free (&error);
      return 1;
    }

  if (!match_acquired_or_lost_signal (conn[0],
                                      "NameAcquired",
                                      dbus_bus_get_unique_name (conn[0])))
    return 1;

  conn[1] = dbus_bus_get_private (DBUS_BUS_SESSION, &error);
  if (dbus_error_is_set (&error))
    {
      fprintf (stderr, "*** Failed to open connection 1 to session bus: %s\n",
               error.message);
      dbus_error_free (&error);
      return 1;
    }

  if (!match_acquired_or_lost_signal (conn[1],
                                      "NameAcquired",
                                      dbus_bus_get_unique_name (conn[1])))
    return 1;


  conn[2] = dbus_bus_get_private (DBUS_BUS_SESSION, &error);
  if (dbus_error_is_set (&error))
    {
      fprintf (stderr, "*** Failed to open connection 2 to session bus: %s\n",
               error.message);
      dbus_error_free (&error);
      return 1;
    }

  if (!match_acquired_or_lost_signal (conn[2],
                                      "NameAcquired",
                                      dbus_bus_get_unique_name (conn[2])))
    return 1;


  conn[3] = dbus_bus_get_private (DBUS_BUS_SESSION, &error);
  if (dbus_error_is_set (&error))
    {
      fprintf (stderr, "*** Failed to open connection 3 to session bus: %s\n",
               error.message);
      dbus_error_free (&error);
      return 1;
    }

  if (!match_acquired_or_lost_signal (conn[3],
                                      "NameAcquired",
                                      dbus_bus_get_unique_name (conn[3])))
    return 1;


  monitor = dbus_bus_get (DBUS_BUS_SESSION, &error);
  if (dbus_error_is_set (&error))
    {
      fprintf (stderr, "*** Failed to open monitoring connection to session bus: %s\n",
               error.message);
      dbus_error_free (&error);
      return 1;
    }

  if (!match_acquired_or_lost_signal (monitor,
                                      "NameAcquired",
                                      dbus_bus_get_unique_name (monitor)))
    return 1;

  dbus_bus_add_match (monitor, "", &error);
  if (dbus_error_is_set (&error))
    {
      fprintf (stderr, "*** Failed to set filter on monitoring connection: %s\n",
               error.message);
      dbus_error_free (&error);
      return 1;
    }


  for (i = 0; i < NUM_CONN; i++)
    dbus_connection_set_exit_on_disconnect (conn[i], FALSE);

  for (i = 0; i < test_data_len; i++)
    {
      dbus_uint32_t result;
      result = 0;

      if (test_data[i].command == ADD_CONNECTION)
        {
          result = dbus_bus_request_name (conn[test_data[i].connection_number],
                                          TEST_NAME,
                                          test_data[i].flags,
                                          &error);

          if (dbus_error_is_set (&error))
            {
              fprintf (stderr, "Error on addition in iteration %i: %s\n", i, error.message);
              dbus_error_free (&error);
              return 1;
            }
        }
      else if (test_data[i].command == REMOVE_CONNECTION)
        {
          result = dbus_bus_release_name (conn[test_data[i].connection_number],
                                          TEST_NAME,
                                          &error);
          if (dbus_error_is_set (&error))
            {
              fprintf (stderr, "*** Failed to remove connection %i in iteration %i: %s\n",
                       test_data[i].connection_number,
                       i,
                       error.message);
              dbus_error_free (&error);
              return 1;
            }
        }
      else
        {
          fprintf (stderr, "Command #%i not a valid command!\n", test_data[i].command);
          return 1;
        }


      if (result != test_data[i].expected_result)
        {
          fprintf (stderr, "Results recived (%i) are not the expected results (%i) in iteration %i\n",
                   result,
                   test_data[i].expected_result,
                   i);
          return 1;
        }

      if (!check_connection (monitor, i, conn))
        {
          fprintf (stderr, "Failed at iteration %i\n", i);
          return 1;
        }

      if (!check_signals (monitor, i, conn))
        {
          fprintf (stderr, "Failed at iteration %i\n", i);
          return 1;
        }
    }

  return 0;
}
