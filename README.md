# RSS Reader

This is an RSS reader application. It allows users to subscribe to RSS feeds, read articles, and manage their subscriptions.

## Features

- **Subscription Management:** Easily subscribe to new RSS feeds and manage existing subscriptions.
- **Article Reading:** Read articles from subscribed feeds within the application.
- **GUI Interface:** Provides a user-friendly graphical interface for interacting with the application.




## Screenshots
|Startup Screen|Read Articles|
|--------------|-------------|
|![one](https://github.com/nivassai03/rss_reader/assets/116424621/1df5bed1-1023-43fc-b398-c9735dfe5ad8)|![two](https://github.com/nivassai03/rss_reader/assets/116424621/689f6446-5eae-445a-a725-097f5800921b)|

|Subscribe feed|Fetch New Articles|
|--------------|------------------|
|![three](https://github.com/nivassai03/rss_reader/assets/116424621/e1e5f88a-6fd3-49fe-85ab-8acfaa3cf8af)|![four](https://github.com/nivassai03/rss_reader/assets/116424621/245bf625-74b6-477d-8c2e-bd3a642999aa)|

| Read from multiple tabs|
|------------------------|
|![five](https://github.com/nivassai03/rss_reader/assets/116424621/65a62f05-5202-49e2-85b7-8f83e44ee2ef)|

## Installation 

> [!Note]
> This project is currently developed and tested on linux.


1. Install dependencies.
```sh
sudo apt install git cmake build-essential libgtk-3-dev libmbedtls-dev libwebkit2gtk-4.0-dev
```
2. Clone the repository.
```sh
git clone https://github.com/nivassai03/rss_reader.git
```
3. Build the application.
```sh
cd build
cmake ..
make
```
4. Run the application.
```sh
./rssreader
```


## Credits
Rss reader is written in c++20 and uses the following open-source packages.
- [wxWidgets](https://www.wxwidgets.org/)
- [nlohmann-json](https://github.com/nlohmann/json)
- [pugixml](https://github.com/zeux/pugixml)
- [cpr](https://github.com/libcpr/cpr)
