const express = require('express');

const router = express.Router();

/* Get all uploaded manager, and select based on query params */
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
  req.context.models.Manager.findAll(query)
    /* Return the JSON array */
    .then((manager) => res.status(200).json(manager))
    /* Handle errors*/
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Get a specific manager point */
router.get('/:managerId', async (req, res) => {
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
  req.context.models.Manager.findByPk(req.params.managerId, query)
    .then((manager) => {
      /* Check if the manager was null */
      if (manager) {
        /* Return the information */
        return res.status(200).json(manager);
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

/* Create a new manager record */
router.post('/', async (req, res) => {
  req.context.models.Manager.bulkCreate(req.body)
    /* Successful insert */
    .then((manager) => res.status(201).json(manager))
    /* Handle Errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Edit a manager */
router.put('/:managerId', async (req, res) => {
  /* Query the database for the group and update its values */
  req.context.models.Manager.update(req.body, {
      where: { id: req.params.managerId }
    })
    .then((n) => {
      /* Check the number of affect rows */
      if (n > 0) {
        /* If a row was affected, return the new data for the whole group */
        req.context.models.Manager.findByPk(req.params.managerId)
          .then((manager) => {
            /* Return the new group */
            return res.status(200).json(manager);
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


/* Remove a manager */
router.delete('/:managerId', async (req, res) => {
  /* Query the db and delete the buoy */
  req.context.models.Manager.destroy({
    where: { id: req.params.managerId }
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
