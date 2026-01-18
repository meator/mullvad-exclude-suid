# mullvad-exclude-suid
This is a small, hacky and likely unsafe program that can be used in place of `mullvad-exclude` to allow running it as a regular user on non-systemd (for example runit) systems.

The soluton is inspired by an old reddit comment by a deleted user: https://old.reddit.com/r/voidlinux/comments/nw8eot/cgroup_permissions_mullvad/iz2m4fs/

The program expects to have the SETUID bit set. It makes the `/sys/fs/cgroup/net_cls/mullvad-exclusions/cgroup.procs` file writable by anyone, it drops its priviledges and it hands its arguments to `mullvad-exclude`, which should be able to work as a regular user once the `cgroup.procs` file becomes writable.

The program handles errors of POSIX library functions responsibly and it is not affected by the environment. The program is small, you are encouraged to read and check it.

## Building
```sh
meson setup build
meson compile -C build
sudo meson install -C build
```

`meson install` sets the SETUID bit of the executable by default. See build options for configuring this behavior.

## License
This project is licensed under `GPL-3.0-only`.
