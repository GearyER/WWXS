const express = require('express');

const router = express.Router();

/* Get all buoys, and select based on query params */
router.get('/', async (req, res) => {
  /* fill out a WHERE clause with query params */
  const query = {};
  query.where = {};
  query.include = [
    {
      model: req.context.models.Group
    }
  ]
  if (req.query.groupId) {
    query.where.groupId = req.query.groupId
  }
  /* begin a SELECT query */
  req.context.models.Buoy.findAll(query)
    /* Return the JSON array */
    .then((buoys) => res.status(200).json(buoys))
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Get a specific buoy */
router.get('/:buoyId', async (req, res) => {
  /* Get the buoy from the private key in the URL parameter */
  req.context.models.Buoy.findByPk(req.params.buoyId, query)
    .then((buoy) => {
      /* Check if the data was null */
      if (buoy) {
        /* Return the information */
        return res.status(200).json(buoy);
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

/* Get current location of a buoy */
router.get('/:buoyId/location', async (req, res) => {
  const buoyId = req.params.buoyId;
  const locationHistory = await req.context.models.History.findOne({
    where: { buoyId: buoyId },
    order: [['createdAt', 'DESC']],
    include: [req.context.models.Location]
  });
  if (locationHistory) {
    return res.status(200).json(locationHistory.Location);
  } else {
    return res.status(404).json({ message: 'Location Not Found for Buoy' });
  }
});

/* Get the owner of a buoy */
router.get('/:buoyId/owner', async (req, res) => {
  /* Get the buoy from the private key in the URL parameter */
  req.context.models.Buoy.findByPk(req.params.buoyId)
    .then((buoy) => {
      /* Check if the data was null */
      if (!buoy) {
        /* Return a 404 error, not found */
        return res.status(404).json({ message: 'Buoy Not Found' });
      }
      /* Find group associated with buoy */
      buoy.getGroup()
      .then((group) => {
        /* Find owner associated with group */
        group.getUsers({ where: { role: 'owner' } })
          .then((users) => {
            /* Validate the buoy owner */
            if (users.length === 0) {
              /* Return a 404 error, not found */
              return res.status(404).json({ message: 'No owner found for the buoy' });
            }
            /* Return the owner */
            return res.status(200).json(users[0]);
          })
      })
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Create a buoy */
router.post('/', async (req, res) => {
  req.context.models.Buoy.create(req.body)
    /* Successful insert */
    .then((buoy) => res.status(201).json(buoy))
    /* Handle Errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Edit a buoy */
router.put('/:buoyId', async (req, res) => {
  /* Query the database for the buoy and update its values */
  req.context.models.Buoy.update(req.body, {
      where: { id: req.params.buoyId }
    })
    .then((n) => {
      /* Check the number of affect rows */
      if (n > 0) {
        /* If a row was affected, return the new data for the whole buoy */
        req.context.models.Buoy.findByPk(req.params.buoyId)
          .then((buoy) => {
            /* Return the new buoy */
            return res.status(200).json(buoy);
          })
          /* Handle errors, this should theoretically never be reached */
          .catch((err) => {
            console.error(err);
            return res.status(400).json({ message: 'Bad Request' });
          });
      }
      else {
        /* There were no buoys with that id */
        res.status(404).json({ message: 'Not Found' });
      }
    })
    /* Handle errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Remove a buoy */
router.delete('/:buoyId', async (req, res) => {
  /* Query the db and delete the buoy */
  req.context.models.Buoy.destroy({
      where: { id: req.params.buoyId }
    })
    .then((n) => {
      /* Check the number of affected rows */
      if (n > 0) {
        /* There were rows affected and the buoy was deleted */
        return res.status(200).json({ message: 'OK' });
      }
      else {
        /* There was no buoy with that id, not found */
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
