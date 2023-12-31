const express = require('express');

const router = express.Router();

/* Get all uploaded history, and select based on query params */
router.get('/', async (req, res) => {
  /* fill out a WHERE clause with query params */
  const query = {};
  query.where = {};
  query.include = [
    { 
      model: req.context.models.Location 
    },
    { 
      model: req.context.models.Buoy 
    }
  ];
  if (req.query.locationId) {
    query.where.locationId = req.query.locationId
  }
  /* begin a SELECT query */
  req.context.models.History.findAll(query)
    /* Return the JSON array */
    .then((history) => res.status(200).json(history))
    /* Handle errors*/
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Get a specific history point */
router.get('/:historyId', async (req, res) => {
  const query = {};
  query.where = {};
  query.include = [
    { 
      model: req.context.models.Location 
    }
  ];
  req.context.models.History.findByPk(req.params.historyId, query)
    .then((history) => {
      /* Check if the history was null */
      if (history) {
        /* Return the information */
        return res.status(200).json(history);
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

/* Get buoy history by buoyId */
router.get('/buoy/:buoyId', async (req, res) => {
  const query = {};
  query.where = { buoyId: req.params.buoyId };
  query.include = [
    { 
      model: req.context.models.Location 
    }
  ];
  req.context.models.History.findAll(query)
    .then((history) => res.status(200).json(history))
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Create a new history record */
router.post('/', async (req, res) => {
  req.context.models.History.bulkCreate(req.body)
    /* Successful insert */
    .then((history) => res.status(201).json(history))
    /* Handle Errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Edit a history */
router.put('/:historyId', async (req, res) => {
  /* Query the database for the group and update its values */
  req.context.models.History.update(req.body, {
      where: { id: req.params.historyId }
    })
    .then((n) => {
      /* Check the number of affect rows */
      if (n > 0) {
        /* If a row was affected, return the new data for the whole group */
        req.context.models.History.findByPk(req.params.historyId)
          .then((history) => {
            /* Return the new group */
            return res.status(200).json(history);
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


/* Remove a history */
router.delete('/:historyId', async (req, res) => {
  /* Query the db and delete the buoy */
  req.context.models.History.destroy({
    where: { id: req.params.historyId }
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
