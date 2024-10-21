import os
import json
import re
import argparse

def extract_trace_event(line, tid, name):
    """
    从一行文本中提取 ts 和 dur，并生成一个 tracing 事件。
    """
    # 使用正则表达式提取 SinceStart 和 Duration
    match = re.search(r'SinceStart (\d+), Call \d+: Duration (\d+) microseconds', line)
    if match:
        # 提取 ts 和 dur
        ts = int(match.group(1))  # SinceStart 对应 ts
        dur = int(match.group(2))  # Duration 对应 dur
        
        # 构造 tracing 事件
        event = {
            "pid": 1,  # 假设是一个进程，可以根据需要修改
            "tid": int(tid),  # 线程 ID
            "ts": ts,  # 事件的开始时间
            "ph": "X",  # 完整事件类型
            "name": name,  # 事件名称
            "dur": dur,  # 事件持续时间
            "args": {}  # 可添加额外信息
        }
        return event
    return None

def parse_stat_files(base_dir):
    """
    遍历指定路径下的所有文件，解析并提取 tracing 事件。
    """
    trace_events = []

    # 遍历 base_dir 目录下的文件和文件夹
    for root, dirs, files in os.walk(base_dir):
        for file in files:
            if file.endswith("_stat.txt"):
                # 提取 tid 和 name
                tid = os.path.basename(root)
                name = file.replace("_stat.txt", "")
                
                # 打开并解析文件
                with open(os.path.join(root, file), 'r') as f:
                    for line in f:
                        event = extract_trace_event(line, tid, name)
                        if event:
                            trace_events.append(event)

    return trace_events

def save_to_json(trace_events, output_file):
    """
    将提取到的 tracing 事件保存为 JSON 格式。
    """
    trace_data = {
        "traceEvents": trace_events
    }
    with open(output_file, 'w') as f:
        json.dump(trace_data, f, indent=4)

if __name__ == "__main__":
    # 创建命令行参数解析器
    parser = argparse.ArgumentParser(description="解析指定路径下的stat文件并生成chrome tracing JSON文件")
    parser.add_argument('input_path', type=str, help="要解析的文件夹路径")
    parser.add_argument('--output', type=str, default="chrome_tracing_output.json", help="输出的JSON文件路径")
    
    # 解析命令行参数
    args = parser.parse_args()

    # 输入路径
    input_path = args.input_path
    # 输出文件路径
    output_file = args.output
    
    # 解析文件并生成 tracing 事件
    trace_events = parse_stat_files(input_path)
    
    # 保存为 JSON 文件
    save_to_json(trace_events, output_file)
    
    print(f"解析完成，tracing 数据已保存到 {output_file}")
