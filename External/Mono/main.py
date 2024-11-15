import os
import shutil
import subprocess

def get_all_dll_files(path):
    """遍历指定路径下的所有 `.dll` 文件"""
    dll_files = []
    for root, dirs, files in os.walk(path):
        for file in files:
            if file.lower().endswith('.dll'):
                dll_files.append(os.path.join(root, file))
    return dll_files

def execute_build(dll_file):
    """执行 build.exe，构建 DLL 文件并返回生成的 DLL 文件路径"""


    # 要加入到 PATH 的临时路径
    temp_path1 = r"D:\Android\Sdk\ndk\26.1.10909125\toolchains\llvm\prebuilt\windows-x86_64\bin"
    temp_path2 = r"D:\vs2022\ide\VC\Tools\MSVC\14.39.33519\bin\Hostx64\x64"

    # 获取当前的 PATH 环境变量
    current_path = os.environ.get("PATH", "")

    # 将新的路径添加到现有的 PATH 中
    new_path = f"{current_path};{temp_path1};{temp_path2}"

    # 设置更新后的 PATH 环境变量
    os.environ["PATH"] = new_path
    # 构造命令：build.exe dllpath
    try:
        print(f"正在处理 {dll_file}...")

        command = r"D:\GameEngine\MonoDemo\mono\bin\mono-sgen.exe --aot=full --path=D:\GameEngine\MonoDemo\mono\raw " +dll_file
        result = subprocess.run(command, capture_output=True, text=True)

        if result.returncode == 0:
            # 生成的文件名是原 DLL 文件名 + ".dll"
            generated_dll_file = f"{dll_file}.dll"
            print(f"成功构建 {dll_file}，生成文件: {generated_dll_file}")
            return generated_dll_file
        else:
            print(f"构建 {dll_file} 失败: {result.stderr}")
            return None
    except Exception as e:
        print(f"执行 build.exe 时出错: {e}")
        return None

def copy_generated_file(generated_file, target_dir):
    """将生成的 DLL 文件复制到指定目标目录"""
    if generated_file and os.path.exists(generated_file):
        try:
            if not os.path.exists(target_dir):
                os.makedirs(target_dir)  # 如果目标目录不存在，创建目录

            # 复制生成的文件到目标目录
            shutil.copy(generated_file, target_dir)
            print(f"文件 {generated_file} 已成功复制到 {target_dir}")
        except Exception as e:
            print(f"复制文件 {generated_file} 到 {target_dir} 时出错: {e}")
    else:
        print(f"生成的文件 {generated_file} 不存在，跳过复制。")

def main():
    source_path = r"D:\GameEngine\MonoDemo\mono\raw"
    target_directory = r"D:\GameEngine\MonoDemo\mono\aot"

    # 获取所有 DLL 文件
    dll_files = get_all_dll_files(source_path)

    if not dll_files:
        print("没有找到任何 DLL 文件。")
        return

    # 遍历所有 DLL 文件
    for dll in dll_files:
        generated_file = execute_build(dll)  # 执行 build.exe
        copy_generated_file(generated_file, target_directory)  # 复制生成的文件

    print("处理完成。")

if __name__ == "__main__":
    main()

