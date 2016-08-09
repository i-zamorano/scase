#!/bin/sh

cp -f builds/plugins/DocumentWriter/debug/libDocumentWriter.dylib working_dirs/plugins/
cp -f builds/plugins/DocumentWriter/debug/libDocumentWriter.dylib dist/data_files/plugins/
ls -la working_dirs/plugins
