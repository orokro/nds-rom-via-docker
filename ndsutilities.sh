#!/usr/bin/env bash

# Nintendo DS docker helpers for this repo.
# Usage:
#	source ./ndsutilities.sh
#	ndsinstall
#	ndsbuild
#	ndsclean

export NDS_DOCKER_IMAGE="devkitpro/devkitarm:latest"

_nds_repo_root() {
	if git rev-parse --show-toplevel >/dev/null 2>&1; then
		git rev-parse --show-toplevel
	else
		pwd
	fi
}

_nds_mount_path() {
	local repo
	repo="$(_nds_repo_root)"

	if command -v cygpath >/dev/null 2>&1; then
		cygpath -w "$repo"
	else
		printf '%s\n' "$repo"
	fi
}

_nds_docker_run() {
	local repo
	local mount_path

	repo="$(_nds_repo_root)"
	mount_path="$(_nds_mount_path)"

	docker run --rm -it \
		-v "${mount_path}:/work" \
		-w /work \
		"${NDS_DOCKER_IMAGE}" \
		"$@"
}

ndsinstall() {
	docker pull "${NDS_DOCKER_IMAGE}"
}

ndsbuild() {
	_nds_docker_run make
}

ndsmake() {
	ndsbuild
}

ndsclean() {
	_nds_docker_run make clean
}

ndsrebuild() {
	ndsclean
	ndsbuild
}

ndsshell() {
	_nds_docker_run bash
}

ndsrom() {
	local repo
	repo="$(_nds_repo_root)"
	printf '%s\n' "${repo}/cubeui.nds"
}

ndshelp() {
	cat <<'EOF'
Available commands:
	ndsinstall		Pull/update the devkitPro Docker image
	ndsbuild		Build the ROM
	ndsmake			Same as ndsbuild
	ndsclean		Clean build outputs
	ndsrebuild		Clean and rebuild
	ndsshell		Open an interactive shell in the build container
	ndsrom			Print the expected ROM path
EOF
}

echo "Loaded Nintendo DS helpers from ndsutilities.sh"
echo "Try: ndshelp"
