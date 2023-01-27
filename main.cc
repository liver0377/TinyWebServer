#include <fstream>
#include <string>

#include "config/config.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
int main(int argc, char *argv[]) {
  // 需要修改的数据库信息,登录名,密码,库名
  std::ifstream input("config/dbconfig.json");
  json data = json::parse(input);
  if (!data.is_object()) {
    throw std::runtime_error("config file error");
  }

  string user = data["user"];
  string passwd = data["passwd"];
  string databasename = data["dbname"];

  // 命令行解析
  Config config;
  config.parse_arg(argc, argv);

  WebServer server;

  // 初始化
  server.init(config.PORT, user, passwd, databasename, config.LOGWrite,
              config.OPT_LINGER, config.TRIGMode, config.sql_num,
              config.thread_num, config.close_log, config.actor_model);

  // 日志
  server.log_write();

  // 数据库
  server.sql_pool();

  // 线程池
  server.thread_pool();

  // 触发模式
  server.trig_mode();

  // 监听
  server.eventListen();

  // 运行
  server.eventLoop();

  return 0;
}