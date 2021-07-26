.PHONY: all

all:
	gcc port/linux/gdb_api.c gdb_break_and_watch_point.c gdb_cmd_func_extapi.c port/linux/tcp_server.c gdb_stub.c gdb_cmd_func.c main.c -w -I. -o gdb_stub