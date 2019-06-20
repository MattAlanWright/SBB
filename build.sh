os=$(uname -s)
if [ $os = Linux ]; then
    g++ -std=c++17 main.cpp Environment.cpp Host.cpp Dataset.cpp Point.cpp Symbiont.cpp Instruction.cpp -o sbb -lpthread
elif [ $os = Darwin ]; then
    g++ -std=c++17 main.cpp Environment.cpp Host.cpp Dataset.cpp Point.cpp Symbiont.cpp Instruction.cpp -o sbb
fi
