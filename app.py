from flask import Flask, request, render_template

app = Flask(__name__)

request_count = 0


@app.route('/')
def index():
    return "Hello there"


@app.route('/do_something', methods=['GET'])
def do_something():
    return "request" + str(request_count)


if __name__ == '__main__':
    app.run(threaded=True)
