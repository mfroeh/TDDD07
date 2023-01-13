{ pkgs ? import <nixpkgs> {} }:
let
    my-python-packages = p: with p; [
        numpy
    ];
    my-python = python3.withPackages my-python-packages;
in my-python.env
