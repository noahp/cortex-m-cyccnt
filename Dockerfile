FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install --no-install-recommends -y \
    clang-14 && \
    ln -s clang-14 /usr/bin/clang && \
    rm -rf /var/lib/apt/lists/*
