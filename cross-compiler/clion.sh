#!/bin/bash
set -e -o pipefail

TARGET="i386-elf"

KFS_DIR="$(pwd)"
PREFIX_DIR="$KFS_DIR/kfs"
IDEA_DIR="$KFS_DIR/../.idea"
CONFIGS_DIR="$IDEA_DIR/runConfigurations"

WORKSPACE="$IDEA_DIR/workspace.xml"

GDB_BASE="$PREFIX_DIR/bin/$TARGET-gdb"
GDB_CUSTOM="$PREFIX_DIR/bin/$TARGET-gdb-clion"

gdb_wrapper() {
	if [[ ! -x "$GDB_BASE" ]]; then
		echo "$GDB_BASE not found, run cross-compiler/install.sh first." >&2
		exit 1
	fi
	if sed --version >/dev/null 2>&1; then
		local unbuffered="-u"
	else
		local unbuffered="-l"
	fi
	cat > "$GDB_CUSTOM" <<EOF
#!/bin/sh
GDB="$GDB_BASE"
case "\$*" in
	*--interpreter*) ;;
	*) exec "\$GDB" "\$@" ;;
esac
sed $unbuffered -E \\
	-e 's/^([0-9]*).*startup-with-shell.*/\1-gdb-set confirm off/' \\
	-e '/^[0-9]*-exec-continue/{' \\
	-e 's/^([0-9]*).*/\1-exec-step-instruction/' \\
	-e ':a' -e 'n' -e 'ba' -e '}' \\
| exec "\$GDB" -iex "set auto-load local-gdbinit off" -ex "set architecture i386:intel" -ex "set disassembly-flavor intel" -ex "set breakpoint pending on" "\$@"
EOF
	chmod +x "$GDB_CUSTOM"
}

native_run_config() {
	cat > "$CONFIGS_DIR/$1.xml" <<EOF
<component name="ProjectRunConfigurationManager">
  <configuration default="false" name="$1" type="CLionNativeAppRunConfigurationType" REDIRECT_INPUT="false" ELEVATE="false" USE_EXTERNAL_CONSOLE="false" EMULATE_TERMINAL="false" PASS_PARENT_ENVS_2="true" PROJECT_NAME="kfs" TARGET_NAME="$1" CONFIG_NAME="$1" version="1" RUN_PATH="srcs/kernel/build/kfs.elf">
    <method v="2">
      <option name="CLION.COMPOUND.BUILD" enabled="true" />
    </method>
  </configuration>
</component>
EOF
}

remote_run_config() {
	cat > "$CONFIGS_DIR/$1-gdb.xml" <<EOF
<component name="ProjectRunConfigurationManager">
  <configuration default="false" name="$1 (gdb)" type="CLion_Remote" version="1" toolchain="KFS i386" remoteCommand="127.0.0.1:1234" symbolFile="\$PROJECT_DIR\$/srcs/kernel/build/kfs.elf" sysroot="\$PROJECT_DIR\$">
    <debugger kind="GDB">$GDB_CUSTOM</debugger>
    <method v="2">
      <option name="RunConfigurationTask" enabled="true" run_configuration_name="$1" run_configuration_type="CLionNativeAppRunConfigurationType" />
    </method>
  </configuration>
</component>
EOF
}

import_configs() {
	rm -rf "$CONFIGS_DIR"
	mkdir -p "$CONFIGS_DIR"
	native_run_config "all"
	native_run_config "debug"
	remote_run_config "debug"
}

clean_configs() {
	[[ -f "$WORKSPACE" ]] || return 0
	sed -e '/<component name="RunManager"/,/^  <\/component>/{
		/^    <configuration name=/,/^    <\/configuration>/d
		/<item itemvalue=/d
	}' "$WORKSPACE" > "$WORKSPACE.tmp"
	mv "$WORKSPACE.tmp" "$WORKSPACE"
}

gdb_wrapper
import_configs
clean_configs

echo -e "\nGo to Settings -> Build, Execution, Deployment -> Toolchains\nAdd a new toolchain with the exact following settings:"
echo -e "  Name: KFS i386\n  CMake: unchanged\n  Build tool: unchanged\n  C Compiler: $PREFIX_DIR/bin/$TARGET-gcc"
echo -e "  C++ Compiler: $PREFIX_DIR/bin/$TARGET-g++\n  Debugger (GDB): $PREFIX_DIR/bin/$TARGET-gdb-clion\n"
echo -e "Go to Settings -> Build, Execution, Deployment -> Debugger -> Debug Profiles\nAdd a new profile with the exact following settings:"
echo -e "  Name: KFS GDB\n  Available for all projetct: no\n  Shared: no\n  Executable: $PREFIX_DIR/bin/$TARGET-gdb-clion"
echo -e "Don't forget to select KFS GDB's debug profile afterwards!\n"
echo -e "Harmless error \"Don't know how to run. Try \"help target\".\" on debug targets expected.\n"
echo -e "Restart CLion, happy coding!\n"
