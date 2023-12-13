const express = require('express');

const router = express.Router();

/* Get all uploaded authorized, and select based on query params */
router.get('/', async (req, res) => {
  /* fill out a WHERE clause with query params */
  const query = {};
  query.where = {};
  query.include = [
    {
      model: req.context.models.Buoy
    },
    {
      model: req.context.models.User
    }
  ]
  if (req.query.buoyId) {
    query.where.buoyId = req.query.buoyId
  }
  if (req.query.userId) {
    query.where.userId = req.query.userId
  }
  /* begin a SELECT query */
  req.context.models.Authorized.findAll(query)
    /* Return the JSON array */
    .then((authorized) => res.status(200).json(authorized))
    /* Handle errors*/
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Get a specific authorized user point */
router.get('/:authorizedId', async (req, res) => {
  const query = {};
  query.where = {};
  query.include = [
    {
      model: req.context.models.Buoy
    },
    {
      model: req.context.models.User
    }
  ]
  if (req.query.buoyId) {
    query.where.buoyId = req.query.buoyId
  }
  if (req.query.userId) {
    query.where.userId = req.query.userId
  }
  req.context.models.Authorized.findByPk(req.params.authorizedId, query)
    .then((authorized) => {
      /* Check if the authorized user was null */
      if (authorized) {
        /* Return the information */
        return res.status(200).json(authorized);
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

/* Create a new authorized user record */
router.post('/', async (req, res) => {
  req.context.models.Authorized.bulkCreate(req.body)
    /* Successful insert */
    .then((authorized) => res.status(201).json(authorized))
    /* Handle Errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Edit a authorized user */
router.put('/:authorizedId', async (req, res) => {
  /* Query the database for the authorized user and update its values */
  req.context.models.Authorized.update(req.body, {
      where: { id: req.params.authorizedId }
    })
    .then((n) => {
      /* Check the number of affect rows */
      if (n > 0) {
        /* If a row was affected, return the new data for the whole authorized user */
        req.context.models.Authorized.findByPk(req.params.authorizedId)
          .then((authorized) => {
            /* Return the new authorized user */
            return res.status(200).json(authorized);
          })
          /* Handle errors, this should theoretically never be reached */
          .catch((err) => {
            console.error(err);
            return res.status(400).json({ message: 'Bad Request' });
          });
      }
      else {
        /* There were no histories with that id */
        return res.status(404).json({ message: 'Not Found' });
      }
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});


/* Remove a authorized user */
router.delete('/:authorizedId', async (req, res) => {
  /* Query the db and delete the buoy */
  req.context.models.Authorized.destroy({
    where: { id: req.params.authorizedId }
  })
  .then((n) => {
    /* Check the number of affected rows */
    if (n > 0) {
      return res.status(200).json({ message: 'OK' });
    } else {
      /* There was no location with that id, not found */
      return res.status(404).json({ message: 'Not Found' });
    }
  })
  /* Handle errors */
  .catch((err) => {
    console.error(err);
    return res.status(400).json({ message: 'Bad Request' });
  })
});


module.exports = router;
