const express = require('express');

const router = express.Router();

const authenticate = require('../helpers/authenticate');
const authorize = require('../helpers/authorize');

/* Get all users */
router.get('/', authorize, async (req, res) => {
  /* begin a SELECT query */
  const query = {};
  query.include = [
    {
      model: req.context.models.Group
    },
    {
      model: req.context.models.Buoy
    }
  ]
  req.context.models.User.findAll(query)
    /* Return the JSON array */
    .then((users) => {
      return res.status(200).json(users)
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Get a specific user */
router.get('/:userId', async (req, res) => {
  const query = {};
  query.include = [
    {
      model: req.context.models.Group
    },
    {
      model: req.context.models.Buoy
    }
  ]
  req.context.models.User.findByPk(req.params.userId, query)
    .then((user) => {
      /* Check if the user was null */
      if (user) {
        /* Return the information */
        return res.status(200).json(user);
      }
      else {
        /* Return a 404 error, not found */
        return res.status(404).json({ message: 'Not Found' });
      }
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* TODO 
 * send and receive hashed usernames
 * This may contain sensitive information which should not be sent
 * as an API request
*/
/* Get buoys owned and managed by a specific user based on username */
router.get('/:username/buoys', async (req, res) => {
  const userQuery = {};
  userQuery.where = { username: req.params.username };
  userQuery.include = [
    {
      model: req.context.models.Buoy
    }
  ] // For owned buoys

  req.context.models.User.findOne(userQuery)
    .then(async (user) => {
      if (user) {
        const managerQuery = {};
        managerQuery.where = { userId: user.id };
        managerQuery.include = [
          {
            model: req.context.models.Buoy
          }
        ] // For managed buoys
        const authorizedQuery = {};
        authorizedQuery.where = { userId: user.id };
        authorizedQuery.include = [
          {
            model: req.context.models.Buoy
          }
        ] // For authorized buoys

        const managers = await req.context.models.Manager.findAll(managerQuery);
        const managedBuoys = managers.map(manager => manager.buoy);

        const authUsers = await req.context.models.Authorized.findAll(authorizedQuery);
        const authorizedBuoys = authUsers.map(authUser => authUser.buoy);

        // Combine owned and managed and authorized buoys
        // const allBuoys = [...user.buoys, ...managedBuoys, ...authorizedBuoys];

        return res.status(200).json({
          ownedBuoys: user.buoys,
          managedBuoys: managedBuoys,
          authorizedBuoys: authorizedBuoys,
          // allBuoys: allBuoys
        });
      } else {
        return res.status(404).json({ message: 'User Not Found' });
      }
    })
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Login, returns JWT Token and user info */
router.post('/login', (req, res, next) => {
  /* Create query */
  const query = {};
  query.where = {};
  query.include = [
    {
      model: req.context.models.Group
    }
  ]
  if (req.body.username && req.body.password) {
    query.where.username = req.body.username;
    query.where.password = req.body.password;
  }
  else {
    return res.status(400).json({ message: 'Bad Request' });
  }
  console.log(query);
  /* Execute query */
  req.context.models.User.findOne(query)
    /* Successful find */
    .then((user) => {
      if (user) {
        user.dataValues.token = authenticate.createToken();
        console.log(user);
        return res.status(200).json(user);
      }
      else {
        return res.status(400).json({ message: 'Bad Request' });
      }
    })
    /* Handle Errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Create a user */
router.post('/', async (req, res) => {
  const query = {};
  query.include = [
    {
      model: req.context.models.Group
    }
  ]
  req.context.models.User.bulkCreate(req.body, query)
    /* Successful insert */
    .then((user) => {
      user.dataValues.token = authenticate.createToken();
      console.log(user);
      return res.status(201).json(user);
    })
    /* Handle Errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

module.exports = router;
