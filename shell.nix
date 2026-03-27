{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    qt6.qtbase
    qt6.qt5compat
    qt6.qttools
    pkg-config
    git
  ];

  shellHook = ''
    echo "OpenJardin"
    echo "mkdir build && cd build && cmake .. && make -j$(nproc)"
    rm -rf ./build
  '';

  CMAKE_PREFIX_PATH = "${pkgs.qt6.qtbase};${pkgs.qt6.qt5compat}";
}
