#!/bin/sh
cd task$1 && cmake -Bbuild -H. && make --directory=build
