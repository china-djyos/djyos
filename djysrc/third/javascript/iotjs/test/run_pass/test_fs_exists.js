/* Copyright 2016-present Samsung Electronics Co., Ltd. and other contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

var fs = require('fs');
var assert = require('assert');

{
  var filePath = "../resources/tobeornottobe.txt";

  fs.exists(filePath, function(exists) {
    assert.equal(exists, true);
  });
}

{
  var filePath = "../resources/empty.txt";

  fs.exists(filePath, function(exists) {
    assert.equal(exists, false);
  });
}

{
  var filePath = "";

  fs.exists(filePath, function(exists) {
    assert.equal(exists, false);
  });
}

{
  var filePath = " ";

  fs.exists(filePath, function(exists) {
    assert.equal(exists, false);
  });
}
