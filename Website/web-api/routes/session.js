const express = require('express');

const router = express.Router();

router.get('/', async (req, res) => {
  res.send(req.context.loggedIn);
});

router.post('/logout', (req, res) => {
  let result = false;
  if (req.context.loggedIn) {
    req.context.loggedIn = null;
    result = true;
  }

  res.send(result);
});

router.post('/login', async (req, res) => {
  let user = null;
  if (!req.context.loggedIn) {
    user = await req.context.models.User.findByLogin(
      req.body.username
    );

    if (user && req.body.password === user.password) { // TODO: Move authentication elsewhere
      req.context.loggedIn = user;
      console.log(`logged in as ${user.username}`);
    }
    else {
      user = null;
      console.log('wrong password or user not found');
    }
  }

  res.send(user);
});

module.exports = router;
