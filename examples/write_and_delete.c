
//
// monotone.
//
// embeddable cloud-native storage for events
// and time-series data.
//

//
// This file is a Public Domain.
//

#include <inttypes.h>
#include <monotone.h>

static void
execute(monotone_t* env, const char* cmd)
{
	printf("> %s\n", cmd);

	// show statistics
	char* result = NULL;
	int rc = monotone_execute(env, cmd, &result);
	if (rc == -1)
		fprintf(stderr, "monotone_execute() failed: %s\n", monotone_error(env));
	if (result)
	{
		printf("%s\n", result);
		free(result);
	}
}

static int
prepare(void)
{
	// create environment
	monotone_t* env = monotone_init();
	if (env == NULL)
	{
		fprintf(stderr, "monotone_init() failed\n");
		return EXIT_FAILURE;
	}

	// repeat log messages to the console
	execute(env, "set log_to_stdout to true");

	// create or open repository
	int rc = monotone_open(env, "./example_repo_2");
	if (rc == -1)
	{
		monotone_free(env);
		fprintf(stderr, "monotone_open() failed: %s\n", monotone_error(env));
		return EXIT_FAILURE;
	}

	// write 10000 events
	const int batch_size = 100;
	monotone_event_t batch[batch_size];

	int id = 0;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < batch_size; j++)
		{
			// set id to UINT64_MAX for the serial mode, id will be assigned
			// automatically
			monotone_event_t* event = &batch[j];
			event->flags      = 0;
			event->id         = id++;
			event->key        = NULL;
			event->key_size   = 0;
			event->value      = NULL;
			event->value_size = 0;
		}
		rc = monotone_write(env, batch, batch_size);
		if (rc == -1)
		{
			monotone_free(env);
			fprintf(stderr, "monotone_write() failed: %s\n", monotone_error(env));
			return EXIT_FAILURE;
		}
	}

	// show statistics
	execute(env, "show partitions");

	monotone_free(env);
	return 0;
}

static int
process_1_by_1(void)
{
	// create environment
	monotone_t* env = monotone_init();
	if (env == NULL)
	{
		fprintf(stderr, "monotone_init() failed\n");
		return EXIT_FAILURE;
	}

	// repeat log messages to the console
	execute(env, "set log_to_stdout to true");

	// create or open repository
	int rc = monotone_open(env, "./example_repo_2");
	if (rc == -1)
	{
		monotone_free(env);
		fprintf(stderr, "monotone_open() failed: %s\n", monotone_error(env));
		return EXIT_FAILURE;
	}

	// show statistics
	execute(env, "show partitions");

	// read all events, starting from zero
	monotone_event_t key =
	{
		.flags      = 0,
		.id         = 0,
		.key        = NULL,
		.key_size   = 0,
		.value      = NULL,
		.value_size = 0
	};
	monotone_cursor_t* cursor = monotone_cursor(env, NULL, &key);
	if (cursor == NULL)
	{
		monotone_free(env);
		fprintf(stderr, "monotone_cursor() failed: %s\n", monotone_error(env));
		return EXIT_FAILURE;
	}

	int total = 0;
	for (;;)
	{
		// read current cursor event
		monotone_event_t event;
		rc = monotone_read(cursor, &event);
		if (rc == -1)
		{
			fprintf(stderr, "monotone_read() failed: %s\n", monotone_error(env));
			break;
		}
		if (rc == 0)
			break;

		// mark event for deletion and delete
		event.flags = MONOTONE_DELETE;
		rc = monotone_write(env, &event, 1);
		if (rc == -1)
		{
			monotone_free(env);
			fprintf(stderr, "monotone_write() failed: %s\n", monotone_error(env));
			return EXIT_FAILURE;
		}

		total++;

		// iterate forward
		rc = monotone_next(cursor);
		if (rc == -1)
		{
			fprintf(stderr, "monotone_next() failed: %s\n", monotone_error(env));
			break;
		}
	}

	printf("total: %d\n\n", total);

	// show statistics
	execute(env, "show partitions");

	// refresh
	execute(env, "refresh partition 0");

	// show statistics
	execute(env, "show partitions");

	monotone_free(cursor);
	monotone_free(env);
	return 0;
}

int
main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	// create, write events and close
	int rc = prepare();
	if (rc != 0)
		return rc;

	// open, read and delete events
	return process_1_by_1();
}
