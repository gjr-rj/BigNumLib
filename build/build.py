import subprocess

def doc_generate():
    subprocess.run(["doxygen.exe", "../Doxyfile"])

def binary_build():
    subprocess.run(["cmake", "../CMakeLists.txt"])
    subprocess.run(["cmake", "--build", ".", "--config", "release"])

def exec_smoke():
    subprocess.run(["../bin/Release/bignumlibQA.exe"])

def main():
    doc_generate()
    binary_build()
    exec_smoke()

if __name__=="__main__":
    main()
