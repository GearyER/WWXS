const jwt = require('jsonwebtoken');

module.exports = {
  createToken,
  validateToken
};

function createToken() {
  var token = jwt.sign({
    exp: Math.floor(Date.now() / 1000) + (60 * 60),
    data: 'wwxs authentication token'
  }, process.env.API_SECRET);
  return token;
}

function validateToken(token) {
  return jwt.verify(token, config.secret);
}