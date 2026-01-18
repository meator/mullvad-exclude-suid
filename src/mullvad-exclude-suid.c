#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "config.h"

#define CGROUPS_PATH "/sys/fs/cgroup/net_cls/mullvad-exclusions/cgroup.procs"

int main(int argc, char ** argv, char ** envp)
{
	if (chmod(CGROUPS_PATH, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWOTH) ==
		-1)
	{
		if (errno != ENOENT)
		{
			fprintf(stderr, "Couldn't chmod o+w " CGROUPS_PATH ": %s\n",
					strerror(errno));
			return EXIT_FAILURE;
		}
	}
	if (getegid() == 0)
	{
		// This shouldn't happen when the executable is SETUID only
		// (not SETGID). This is the default recommended configuration.
		// But if the user set SETGID, let's drop group priviledges too.
		if (setgid(getgid()) == -1)
		{
			fprintf(stderr, "Couldn't drop root priviledges: %s\n",
					strerror(errno));
			return EXIT_FAILURE;
		}
	}
	if (setuid(getuid()) == -1)
	{
		fprintf(stderr, "Couldn't drop root priviledges: %s\n",
				strerror(errno));
		return EXIT_FAILURE;
	}

	const char ** new_argv = malloc((argc + 1) * sizeof(const char *));
	new_argv[0] = MULLVAD_EXCLUDE_PATH;
	for (int i = 1; i < argc; ++i)
		new_argv[i] = argv[i];
	new_argv[argc] = NULL;

	errno = 0;
	execve(MULLVAD_EXCLUDE_PATH, (char **)new_argv, envp);
	fprintf(stderr, "execve(" MULLVAD_EXCLUDE_PATH ") failed: %s\n",
			strerror(errno));
	return EXIT_FAILURE;
}
