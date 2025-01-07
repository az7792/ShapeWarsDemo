from flask import Flask, send_from_directory, render_template
import os

app = Flask(__name__)

# 配置静态文件路径
@app.route('/js/<path:filename>')
def serve_js(filename):
    return send_from_directory(os.path.join(app.root_path, 'static', 'js'), filename)

# 为 HTML 文件创建一个路由
@app.route('/')
def index():
    return send_from_directory(os.path.join(app.root_path, 'static'), 'Tank.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=7777)

