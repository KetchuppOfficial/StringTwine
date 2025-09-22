{
    description = "Implementation of a template class for fast concatenation of string";

    inputs = {
        nixpkgs.url = "github:nixos/nixpkgs/nixos-25.05";
    };

    outputs = {
        nixpkgs,
        ...
    }: let
        system = "x86_64-linux";
    in {
        devShells."${system}".default = let
            pkgs = import nixpkgs {
                inherit system;
            };
        in (pkgs.mkShell.override { stdenv = pkgs.gcc15Stdenv; }) {
            buildInputs = with pkgs; [
                gtest
                gbenchmark
            ];
            nativeBuildInputs = with pkgs; [
                cmake
            ];
        };
    };
}
