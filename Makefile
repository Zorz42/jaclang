
make: src/*.cpp
  g++ src/*.cpp
  mv a.out jaclang
  sudo cp jaclang /usr/bin/jaclang
  echo Jaclang compiled sucsessfully!
