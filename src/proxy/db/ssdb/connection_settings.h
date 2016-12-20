/*  Copyright (C) 2014-2016 FastoGT. All right reserved.

    This file is part of FastoNoSQL.

    FastoNoSQL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FastoNoSQL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FastoNoSQL.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>  // for string

#include <common/net/types.h>  // for HostAndPort

#include "proxy/connection_settings/iconnection_settings_remote.h"

#include "core/db/ssdb/config.h"  // for Config

namespace fastonosql {
namespace proxy {
namespace ssdb {

class ConnectionSettings : public IConnectionSettingsRemote {
 public:
  explicit ConnectionSettings(const connection_path_t& connectionName);

  Config Info() const;
  void SetInfo(const Config& info);

  virtual std::string Delimiter() const override;
  virtual void SetDelimiter(const std::string& delimiter) override;

  virtual std::string NsSeparator() const override;
  virtual void SetNsSeparator(const std::string& ns) override;

  virtual common::net::HostAndPort Host() const override;
  virtual void SetHost(const common::net::HostAndPort& host) override;

  virtual std::string CommandLine() const override;
  virtual void SetCommandLine(const std::string& line) override;

  virtual ConnectionSettings* Clone() const override;

 private:
  Config info_;
};

}  // namespace ssdb
}  // namespace proxy
}  // namespace fastonosql
