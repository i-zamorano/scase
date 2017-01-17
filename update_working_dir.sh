#!/bin/sh

cp -f builds/plugins/DocumentWriter/osx/debug/libDocumentWriter.dylib working_dirs/plugins/
cp -f builds/plugins/DocumentWriter/osx/debug/libDocumentWriter.dylib dist/data/plugins/
cp -f builds/plugins/FileManager/osx/debug/libFileManager.dylib working_dirs/plugins/
cp -f builds/plugins/FileManager/osx/debug/libFileManager.dylib dist/plugins/
ls -la working_dirs/plugins
