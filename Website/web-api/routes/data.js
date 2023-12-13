const express = require('express');

const router = express.Router();

/* Get all uploaded data, and select based on query params */
router.get('/', async (req, res) => {
  /* fill out a WHERE clause with query params */
  const query = {};
  query.where = {};
  query.include = [
    {
      model: req.context.models.Location
    }
  ]
  if (req.query.locationId) {
    query.where.locationId = req.query.locationId
  }
  /* begin a SELECT query */
  req.context.models.Data.findAll(query)
    /* Return the JSON array */
    .then((data) => res.status(200).json(data))
    /* Handle errors*/
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Get a specific data point */
router.get('/:dataId', async (req, res) => {
  const query = {};
  query.include = [
    {
      model: req.context.models.Location
    }
  ]
  req.context.models.Data.findByPk(req.params.dataId, query)
    .then((data) => {
      /* Check if the data was null */
      if (data) {
        /* Return the information */
        return res.status(200).json(data);
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

/* Create a new data record */
router.post('/', async (req, res) => {
  req.context.models.Data.bulkCreate(req.body)
    /* Successful insert */
    .then((data) => res.status(201).json(data))
    /* Handle Errors */
    .catch((err) => {
      console.error(err);
      return res.status(400).json({ message: 'Bad Request' });
    });
});

/* Based on most use cases and for security, changing and deleting data should be left out for now */

module.exports = router;
