import urllib3
import time

start_time = round(time.time() * 1000)
last_time = round(time.time() * 1000)

print(start_time)


def retrieve_something():
    hdr = {
        'User-Agent':
            'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11',
        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
        'Accept-Charset': 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
        'Accept-Encoding': 'none',
        'Accept-Language': 'en-US,en;q=0.8',
        'Connection': 'keep-alive'}

    html_string = urllib3.PoolManager().request("GET", "http://127.0.0.1:5000/do_something", headers=hdr)
    return html_string.data


for i in range(0, 50000):
    print(retrieve_something())
    print(" / " + str(last_time - start_time))
    last_time = round(time.time() * 1000)
