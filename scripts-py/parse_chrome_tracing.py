import os
import json
import re
import argparse
from icecream import ic

def extract_stat_trace_event(line, pid, tid, name):
    """
    从一行文本中提取 ts 和 dur，并生成一个 tracing 事件。
    """
    # 使用正则表达式提取 SinceStart 和 Duration
    match = re.search(r'SinceStart (\d+), Call (\d+): Duration (\d+) microseconds', line)
    if match:
        # 提取 ts 和 dur
        ts = int(match.group(1))  # SinceStart 对应 ts
        Call = int(match.group(2))  # Duration 对应 dur
        dur = int(match.group(3))  # Duration 对应 dur
        
        # 构造 tracing 事件
        event = {
            "pid": pid,  # 假设是一个进程，可以根据需要修改
            "tid": int(tid),  # 线程 ID
            "ts": ts,  # 事件的开始时间
            "ph": "X",  # 完整事件类型
            "name": name,  # 事件名称
            "dur": dur,  # 事件持续时间
            "args": {
                "info": f'Call {Call}'
            }  # 可添加额外信息
        }
        return event
    return None


def extract_record_trace_event(line, pid, tid):
    """
    从一行文本中提取 ts 和 dur，并生成一个 tracing 事件。
    """
    ic(line, tid)
    # 使用正则表达式提取 SinceStart 和 Duration
    match = re.search(r'SinceStart (\d+), Call \d+: Duration (\d+) microseconds.\(1e-6 s\), record: (.*)', line)
    if match:
        ic("got match!!!")
        # 提取 ts 和 dur
        ts = int(match.group(1))  # SinceStart 对应 ts
        dur = int(match.group(2))  # Duration 对应 dur
        info = str(match.group(3))  # Duration 对应 dur
        
        # 构造 tracing 事件
        event = {
            "pid": pid,  # 假设是一个进程，可以根据需要修改
            "tid": int(tid),  # 线程 ID
            "ts": ts,  # 事件的开始时间
            "ph": "X",  # 完整事件类型
            "name": "record",  # 事件名称
            "dur": dur,  # 事件持续时间
            "args": {
                "info": info
            }
        }
        return event
    return None


def extract_paths_from_log(log_file):
    """
    从 log 文件中提取不重复的 /tmp/cpp_{pid} 路径。
    """
    paths = set()
    pattern = re.compile(r'/tmp/cpp_\d+')
    
    with open(log_file, 'r') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                paths.add(match.group(0))
    
    return list(paths)

def parse_files(base_dir):
    """
    遍历指定路径下的所有文件，解析并提取 tracing 事件。
    """
    trace_events = []

    pattern = re.compile(r'/tmp/cpp_(\d+)')
    match = pattern.search(base_dir)
    pid = match.group(1) 

    # 遍历 base_dir 目录下的文件和文件夹
    for root, dirs, files in os.walk(base_dir):
        tid = os.path.basename(root)
        for file in files:
            if file.endswith("_stat.txt"):
                # 提取 tid 和 name
                name = file.replace("_stat.txt", "")
                
                # 打开并解析文件
                with open(os.path.join(root, file), 'r') as f:
                    for line in f:
                        event = extract_stat_trace_event(line, pid, tid, name)
                        if event:
                            trace_events.append(event)
            if file.endswith("_record.txt"):
                # 打开并解析文件
                with open(os.path.join(root, file), 'r') as f:
                    for line in f:
                        event = extract_record_trace_event(line, pid, tid)
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
    parser.add_argument('--input_path', type=str, help="要解析的文件夹路径")
    parser.add_argument('--inputlogfile', type=str, help="包含路径信息的日志文件")
    parser.add_argument('--output', type=str, default="chrome_tracing_output.json", help="输出的JSON文件路径")
    parser.add_argument('--debug', action='store_true', help="开启调试模式")

    # 解析命令行参数
    args = parser.parse_args()

    # 根据 debug 参数开启或关闭 ic
    if args.debug:
        ic.enable()
    else:
        ic.disable()

    trace_events = []
    
    if args.inputlogfile:
        # 从日志文件中提取所有的 /tmp/cpp_{pid} 路径
        paths = extract_paths_from_log(args.inputlogfile)
        ic(paths)
        for path in paths:
            trace_events.extend(parse_files(path))
    elif args.input_path:
        # 使用单个路径进行解析
        trace_events = parse_files(args.input_path)
    else:
        print("请提供 --input_path 或 --inputlogfile 参数。")
        exit(1)

    # 保存为 JSON 文件
    save_to_json(trace_events, args.output)
    
    print(f"解析完成，tracing 数据已保存到 {args.output}")

