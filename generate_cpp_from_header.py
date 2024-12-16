import os

def generate_cpp_files(folder_path):
    """
    指定されたフォルダ内にあるヘッダファイルと同じ名前のcppファイルを生成します。

    Args:
        folder_path (str): 対象フォルダのパス。
    """
    # パス内のバックスラッシュをスラッシュに変換
    folder_path = folder_path.replace('\\', '/')

    if not os.path.isdir(folder_path):
        print(f"指定されたパスはフォルダではありません: {folder_path}")
        return

    # フォルダ内のファイルを取得
    for file_name in os.listdir(folder_path):
        # ヘッダファイルのみを対象とする
        if file_name.endswith('.h') or file_name.endswith('.hpp'):
            # 拡張子を.cppに変更
            base_name = os.path.splitext(file_name)[0]
            cpp_file_name = f"{base_name}.cpp"
            cpp_file_path = os.path.join(folder_path, cpp_file_name)

            # cppファイルが既に存在する場合はスキップ
            if os.path.exists(cpp_file_path):
                print(f"既に存在します: {cpp_file_name}")
                continue

            # 新しいcppファイルを作成
            with open(cpp_file_path, 'w') as cpp_file:
                cpp_file.write(f"// {cpp_file_name}\n")
                cpp_file.write(f"// {file_name} に対応するソースファイル\n\n")
                cpp_file.write(f"#include \"{file_name}\"\n\n")

            print(f"生成しました: {cpp_file_name}")

# 使用例
folder_path = input("folder_path : ")  # 対象フォルダのパスを指定
generate_cpp_files(folder_path)
