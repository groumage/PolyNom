# /bin/bash

version=$(git-conventional-commits version)
echo "New version: $version"

# update project version in files
sed -i "s/[0-9]\+\(\.[0-9]\+\)\.\+[0-9]/$version/g" README.md

git add README.md
git commit -m "build: bump project version to $version"
git-conventional-commits changelog --release $version --file 'CHANGELOG.md'
git add CHANGELOG.md
git commit -m "docs: create $version change log entry"
git tag "v$version"