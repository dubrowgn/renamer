#! /bin/bash

version="$1"
if [[ "$version" == "" ]]; then
	echo "Please provide a version (e.g. '1.4.0-0')"
	exit 1
fi

script_path="$(cd "$(dirname "$0")" && pwd)"

if [[ ! -f "$script_path/pkg/usr/local/bin/renamer" ]]; then
	echo "Binary is missing"
	exit 1
fi

sed -i "s/Version:.*/Version: $version/" "$script_path/pkg/DEBIAN/control" \
	&& dpkg-deb --build pkg \
	&& mv pkg.deb "renamer-${version}_amd64.deb"
