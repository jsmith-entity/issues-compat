{
  description = "cpp dev shell";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self , nixpkgs ,... }: let
    # system should match the system you are running on
    system = "x86_64-linux";
  in {
    devShells."${system}".default = let
      pkgs = import nixpkgs { inherit system; };
    in pkgs.mkShell {
      # create an environment with nodejs_24, pnpm, and yarn
      packages = with pkgs; [
        gcc
		gnumake
		ccls
      ];

      shellHook = ''
        alias c="clear; ls"
        gcc -v
      '';
    };
  };
}
