/*
 * (C) Copyright 2005- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * In applying this licence, ECMWF does not waive the privileges and immunities granted to it by
 * virtue of its status as an intergovernmental organisation nor does it submit to any jurisdiction.
 */

/******************************
 *  Enrico Fucile
 *****************************/

#include "grib_api_internal.h"

/*
   This is used by make_class.pl

   START_CLASS_DEF
   CLASS      = accessor
   SUPER      = grib_accessor_class_gen
   IMPLEMENTS = init
   IMPLEMENTS = unpack_double
   IMPLEMENTS = pack_double
   IMPLEMENTS = value_count
   IMPLEMENTS = get_native_type
   MEMBERS=const char*  missing_value
   MEMBERS=const char*  number_of_values
   MEMBERS=const char*  number_of_points
   MEMBERS=const char*  latitude_of_first_point
   MEMBERS=const char*  ni

   END_CLASS_DEF
 */

/* START_CLASS_IMP */

/*

Don't edit anything between START_CLASS_IMP and END_CLASS_IMP
Instead edit values between START_CLASS_DEF and END_CLASS_DEF
or edit "accessor.class" and rerun ./make_class.pl

*/

static int get_native_type(grib_accessor*);
static int pack_double(grib_accessor*, const double* val, size_t* len);
static int unpack_double(grib_accessor*, double* val, size_t* len);
static int value_count(grib_accessor*, long*);
static void init(grib_accessor*, const long, grib_arguments*);

typedef struct grib_accessor_gds_not_present_bitmap
{
    grib_accessor att;
    /* Members defined in gen */
    /* Members defined in gds_not_present_bitmap */
    const char*  missing_value;
    const char*  number_of_values;
    const char*  number_of_points;
    const char*  latitude_of_first_point;
    const char*  ni;
} grib_accessor_gds_not_present_bitmap;

extern grib_accessor_class* grib_accessor_class_gen;

static grib_accessor_class _grib_accessor_class_gds_not_present_bitmap = {
    &grib_accessor_class_gen,                      /* super */
    "gds_not_present_bitmap",                      /* name */
    sizeof(grib_accessor_gds_not_present_bitmap),  /* size */
    0,                           /* inited */
    0,                           /* init_class */
    &init,                       /* init */
    0,                  /* post_init */
    0,                    /* destroy */
    0,                       /* dump */
    0,                /* next_offset */
    0,              /* get length of string */
    &value_count,                /* get number of values */
    0,                 /* get number of bytes */
    0,                /* get offset to bytes */
    &get_native_type,            /* get native type */
    0,                /* get sub_section */
    0,               /* pack_missing */
    0,                 /* is_missing */
    0,                  /* pack_long */
    0,                /* unpack_long */
    &pack_double,                /* pack_double */
    0,                 /* pack_float */
    &unpack_double,              /* unpack_double */
    0,               /* unpack_float */
    0,                /* pack_string */
    0,              /* unpack_string */
    0,          /* pack_string_array */
    0,        /* unpack_string_array */
    0,                 /* pack_bytes */
    0,               /* unpack_bytes */
    0,            /* pack_expression */
    0,              /* notify_change */
    0,                /* update_size */
    0,             /* preferred_size */
    0,                     /* resize */
    0,      /* nearest_smaller_value */
    0,                       /* next accessor */
    0,                    /* compare vs. another accessor */
    0,      /* unpack only ith value (double) */
    0,       /* unpack only ith value (float) */
    0,  /* unpack a given set of elements (double) */
    0,   /* unpack a given set of elements (float) */
    0,     /* unpack a subarray */
    0,                      /* clear */
    0,                 /* clone accessor */
};


grib_accessor_class* grib_accessor_class_gds_not_present_bitmap = &_grib_accessor_class_gds_not_present_bitmap;

/* END_CLASS_IMP */

static void init(grib_accessor* a, const long v, grib_arguments* args)
{
    int n                                      = 0;
    grib_accessor_gds_not_present_bitmap* self = (grib_accessor_gds_not_present_bitmap*)a;
    grib_handle* hand = grib_handle_of_accessor(a);

    self->missing_value           = grib_arguments_get_name(hand, args, n++);
    self->number_of_values        = grib_arguments_get_name(hand, args, n++);
    self->number_of_points        = grib_arguments_get_name(hand, args, n++);
    self->latitude_of_first_point = grib_arguments_get_name(hand, args, n++);
    self->ni                      = grib_arguments_get_name(hand, args, n++);
    a->length                     = 0;
}

static int value_count(grib_accessor* a, long* number_of_points)
{
    grib_accessor_gds_not_present_bitmap* self = (grib_accessor_gds_not_present_bitmap*)a;
    *number_of_points = 0;
    return grib_get_long_internal(grib_handle_of_accessor(a), self->number_of_points, number_of_points);
}

static int unpack_double(grib_accessor* a, double* val, size_t* len)
{
    grib_accessor_gds_not_present_bitmap* self = (grib_accessor_gds_not_present_bitmap*)a;
    grib_handle* hand = grib_handle_of_accessor(a);

    long number_of_points = 0, number_of_values = 0, ni = 0;
    long latitude_of_first_point = 0;
    size_t i                     = 0;
    size_t n_vals                = 0;
    long nn                      = 0;
    long missing_value;

    int err            = 0;
    double* coded_vals = NULL;
    err                = grib_value_count(a, &nn);
    n_vals             = nn;
    if (err)
        return err;

    if ((err = grib_get_long(hand, self->number_of_points, &number_of_points)) != GRIB_SUCCESS)
        return err;

    if ((err = grib_get_long(hand, self->number_of_values, &number_of_values)) != GRIB_SUCCESS)
        return err;

    if ((err = grib_get_long(hand, self->latitude_of_first_point, &latitude_of_first_point)) != GRIB_SUCCESS)
        return err;

    if ((err = grib_get_long(hand, self->missing_value, &missing_value)) != GRIB_SUCCESS)
        return err;

    if ((err = grib_get_long(hand, self->ni, &ni)) != GRIB_SUCCESS)
        return err;

    if (*len < number_of_points) {
        *len = n_vals;
        return GRIB_ARRAY_TOO_SMALL;
    }

    if (number_of_values > 0) {
        coded_vals = (double*)grib_context_malloc(a->context, number_of_values * sizeof(double));

        if (coded_vals == NULL)
            return GRIB_OUT_OF_MEMORY;
    }

    if (latitude_of_first_point == 0) {
        for (i = 0; i < number_of_values; i++)
            val[i] = 1;
        for (i = number_of_values; i < number_of_points; i++)
            val[i] = 0;
    }
    else {
        for (i = 0; i < ni - 1; i++)
            val[i] = 0;
        for (i = ni - 1; i < number_of_points; i++)
            val[i] = 1;
    }

    *len = number_of_points;

    grib_context_free(a->context, coded_vals);
    return err;
}

static int pack_double(grib_accessor* a, const double* val, size_t* len)
{
    // See deprecated/grib_accessor_class_gds_not_present_bitmap.cc for
    // a possible implementation
    return GRIB_NOT_IMPLEMENTED;
}

static int get_native_type(grib_accessor* a)
{
    return GRIB_TYPE_DOUBLE;
}