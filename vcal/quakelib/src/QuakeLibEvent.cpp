// Copyright (c) 2012 Eric Heien <emheien@ucdavis.edu>
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "QuakeLib.h"
#include <iomanip>

void quakelib::EQSimEventWriter::flush(void) {
	std::vector<unsigned int>::const_iterator	it;
	
	quakelib::EQSimMetadataWriter::write();
	
	for (it=entries_to_flush.begin();it!=entries_to_flush.end();++it) {
		event_summaries.at(*it).write(out_stream);
	}
	
	entries_to_flush.clear();
	out_stream.flush();
}

void quakelib::EQSimEventSummary::parse(const int &line_num, std::istringstream &line_stream) {
	_line_num = line_num;
	
	line_stream >> _event_id	// Field 1: Event ID number (positive integers, in order, need not be consecutive)
	>> _magnitude		// Field 2: Event magnitude
	>> _time			// Field 3: Starting time of event (seconds)
	>> _duration		// Field 4: Duration of event (seconds)
	>> _sid				// Field 5: Fault section ID number (positive integer)
	>> _depth_lo		// Field 6: Lowest value of depth in the rupture (meters, negative underground)
	>> _depth_hi		// Field 7: Highest value of depth in the rupture (meters, negative underground)
	>> _das_lo			// Field 8: Lowest value of distance-along-strike in the rupture (meters)
	>> _das_hi			// Field 9: Highest value of distance-along-strike in the rupture (meters)
	>> _hypo_depth		// Field 10: Hypocenter depth (meters, negative underground)
	>> _hypo_das		// Field 11: Hypocenter distance-along-strike (meters)
	>> _area			// Field 12: Rupture area (square meters)
	>> _mean_slip		// Field 13: Average slip (meters)
	>> _moment			// Field 14: Seismic moment (Newton-meters)
	>> _shear_before	// Field 15: Shear stress before event (Pascal)
	>> _shear_after		// Field 16: Shear stress after event (Pascal)
	>> _normal_before	// Field 17: Normal stress before event (Pascal)
	>> _normal_after;	// Field 18: Normal stress after event (Pascal)
}

void quakelib::EQSimEventReader::parse_event_summary(const int &line_num, std::istringstream &line_stream) {
	quakelib::EQSimEventSummary	es;
	
	es.parse(line_num, line_stream);
	
	event_summaries.push_back(es);
}

void quakelib::EQSimEventSlipMap::parse(const int &line_num, std::istringstream &line_stream) {
	_line_num = line_num;
	
	line_stream >> _depth_lo		// Field 1: Lowest value of depth (meters, negative underground)
				>> _depth_hi		// Field 2: Highest value of depth in the rupture (meters, negative underground)
				>> _das_lo			// Field 3: Lowest value of distance-along-strike in the rupture (meters)
				>> _das_hi			// Field 4: Highest value of distance-along-strike in the rupture (meters)
				>> _area			// Field 5: Rupture area (square meters)
				>> _mean_slip		// Field 6: Average slip (meters)
				>> _moment			// Field 7: Seismic moment (Newton-meters)
				>> _shear_before	// Field 8: Shear stress before event (Pascal)
				>> _shear_after		// Field 9: Shear stress after event (Pascal)
				>> _normal_before	// Field 10: Normal stress before event (Pascal)
				>> _normal_after	// Field 11: Normal stress after event (Pascal)
				>> _element_id;		// Field 12: Element ID number (positive integer), or negative of element count (zero if no element info)
}

void quakelib::EQSimEventReader::parse_event_slip_map(const int &line_num, std::istringstream &line_stream) {
	quakelib::EQSimEventSlipMap	sm;
	
	if (event_summaries.size() == 0) throw std::exception();
	
	sm.parse(line_num, line_stream);

	event_summaries.back().add_slip_map(sm);
}

void quakelib::EQSimEventReader::parse_event_slip_element(const int &line_num, std::istringstream &line_stream) {
	unsigned int		elem_id;
	
	if (event_summaries.size() == 0 || event_summaries.back().slip_maps.size() == 0) throw std::exception();
	
	line_stream >> elem_id;
	
	event_summaries.back().slip_maps.back().add_slip_entry(EQSimEventSlipElement(elem_id, line_num));
}

void quakelib::EQSimEventSummary::write(std::ostream &out_stream) const {
	std::vector<EQSimEventSlipMap>::const_iterator		it;
	std::streamsize										old_prec;
	
	old_prec = out_stream.precision();
	out_stream	<< "200"
				<< " " << _event_id			// Field 1: Event ID number (positive integers, in order, need not be consecutive)
				<< " " << _magnitude			// Field 2: Event magnitude
				<< " " << std::setprecision(16)
				<< _time						// Field 3: Starting time of event (seconds)
				<< " " << std::setprecision(old_prec)
				<< _duration					// Field 4: Duration of event (seconds)
				<< " " << _sid				// Field 5: Fault section ID number (positive integer)
				<< " " << _depth_lo			// Field 6: Lowest value of depth in the rupture (meters, negative underground)
				<< " " << _depth_hi			// Field 7: Highest value of depth in the rupture (meters, negative underground)
				<< " " << _das_lo			// Field 8: Lowest value of distance-along-strike in the rupture (meters)
				<< " " << _das_hi			// Field 9: Highest value of distance-along-strike in the rupture (meters)
				<< " " << _hypo_depth		// Field 10: Hypocenter depth (meters, negative underground)
				<< " " << _hypo_das			// Field 11: Hypocenter distance-along-strike (meters)
				<< " " << _area				// Field 12: Rupture area (square meters)
				<< " " << _mean_slip			// Field 13: Average slip (meters)
				<< " " << _moment			// Field 14: Seismic moment (Newton-meters)
				<< " " << _shear_before		// Field 15: Shear stress before event (Pascal)
				<< " " << _shear_after		// Field 16: Shear stress after event (Pascal)
				<< " " << _normal_before		// Field 17: Normal stress before event (Pascal)
				<< " " << _normal_after		// Field 18: Normal stress after event (Pascal)
				<< "\n";
	
	for (it=slip_maps.begin();it!=slip_maps.end();++it) {
		it->write(out_stream);
	}
}

void quakelib::EQSimEventSlipMap::write(std::ostream &out_stream) const {
	std::vector<EQSimEventSlipElement>::const_iterator		it;
	
	out_stream	<< "201"
				<< " " << _depth_lo			// Field 1: Lowest value of depth (meters, negative underground)
				<< " " << _depth_hi			// Field 2: Highest value of depth in the rupture (meters, negative underground)
				<< " " << _das_lo			// Field 3: Lowest value of distance-along-strike in the rupture (meters)
				<< " " << _das_hi			// Field 4: Highest value of distance-along-strike in the rupture (meters)
				<< " " << _area				// Field 5: Rupture area (square meters)
				<< " " << _mean_slip		// Field 6: Average slip (meters)
				<< " " << _moment			// Field 7: Seismic moment (Newton-meters)
				<< " " << _shear_before		// Field 8: Shear stress before event (Pascal)
				<< " " << _shear_after		// Field 9: Shear stress after event (Pascal)
				<< " " << _normal_before	// Field 10: Normal stress before event (Pascal)
				<< " " << _normal_after;	// Field 11: Normal stress after event (Pascal)
	
	// Field 12: Element ID number (positive integer), or negative of element count (zero if no element info)
	if (slip_elements.size() == 0) {
		out_stream << " " << 0 << "\n";
	} else if (slip_elements.size() == 1) {
		out_stream << " " << slip_elements.back().element_id() << "\n"; 
	} else {
		out_stream << " -" << slip_elements.size() << "\n";
		for (it=slip_elements.begin();it!=slip_elements.end();++it) it->write(out_stream);
	}
}

void quakelib::EQSimEventSlipElement::write(std::ostream &out_stream) const {
	out_stream	<< "202"
				<< " " << _element_id
				<< "\n";
}

bool quakelib::EQSimEventReader::parse_line(const int &rec_num, const int &line_num, std::istringstream &line_stream) {
	switch (rec_num) {
		case 200: parse_event_summary(line_num, line_stream); return true;
		case 201: parse_event_slip_map(line_num, line_stream); return true;
		case 202: parse_event_slip_element(line_num, line_stream); return true;
		default: return false;
	}
}

quakelib::EQSimEventWriter::EQSimEventWriter(void) {
	quakelib::internal::RecordDesc	event_summary_rec, slip_map_rec, slip_elem_rec;
	
	set_spec_level(2);
	meta_add_record(META_SIGNATURE, "EQSim_Output_Event_2");
	
	event_summary_rec = internal::RecordDesc(0, "event_summary", 18, "Record 200: Event summary");
	event_summary_rec.add_field(1, internal::FieldDesc("event_id", 1, 1, "Event ID number (positive integers, in order, need not be consecutive)"));
	event_summary_rec.add_field(2, internal::FieldDesc("magnitude", 2, 2, "Event magnitude"));
	event_summary_rec.add_field(3, internal::FieldDesc("time", 2, 3, "Starting time of event (seconds)"));
	event_summary_rec.add_field(4, internal::FieldDesc("duration", 2, 4, "Duration of event (seconds)"));
	event_summary_rec.add_field(5, internal::FieldDesc("sid", 1, 5, "Fault section ID number (positive integer)"));
	event_summary_rec.add_field(6, internal::FieldDesc("depth_lo", 2, 6, "Lowest value of depth in the rupture (meters, negative underground)"));
	event_summary_rec.add_field(7, internal::FieldDesc("depth_hi", 2, 7, "Highest value of depth in the rupture (meters, negative underground)"));
	event_summary_rec.add_field(8, internal::FieldDesc("das_lo", 2, 8, "Lowest value of distance-along-strike in the rupture (meters)"));
	event_summary_rec.add_field(9, internal::FieldDesc("das_hi", 2, 9, "Highest value of distance-along-strike in the rupture (meters)"));
	event_summary_rec.add_field(10, internal::FieldDesc("hypo_depth", 2, 10, "Hypocenter depth (meters, negative underground)"));
	event_summary_rec.add_field(11, internal::FieldDesc("hypo_das", 2, 11, "Hypocenter distance-along-strike (meters)"));
	event_summary_rec.add_field(12, internal::FieldDesc("area", 2, 12, "Rupture area (square meters)"));
	event_summary_rec.add_field(13, internal::FieldDesc("mean_slip", 2, 13, "Average slip (meters)"));
	event_summary_rec.add_field(14, internal::FieldDesc("moment", 2, 14, "Seismic moment (Newton-meters)"));
	event_summary_rec.add_field(15, internal::FieldDesc("shear_before", 2, 15, "Shear stress before event (Pascal)"));
	event_summary_rec.add_field(16, internal::FieldDesc("shear_after", 2, 16, "Shear stress after event (Pascal)"));
	event_summary_rec.add_field(17, internal::FieldDesc("normal_before", 2, 17, "Normal stress before event (Pascal)"));
	event_summary_rec.add_field(18, internal::FieldDesc("normal_after", 2, 18, "Normal stress after event (Pascal)"));

	slip_map_rec = internal::RecordDesc(0, "slip_map", 12, "Record 201: Slip map entry");
	slip_map_rec.add_field(1, internal::FieldDesc("depth_lo", 2, 1, "Lowest value of depth (meters, negative underground)"));
	slip_map_rec.add_field(2, internal::FieldDesc("depth_hi", 2, 2, "Highest value of depth (meters, negative underground)"));
	slip_map_rec.add_field(3, internal::FieldDesc("das_lo", 2, 3, "Lowest value of distance-along-strike (meters)"));
	slip_map_rec.add_field(4, internal::FieldDesc("das_hi", 2, 4, "Highest value of distance-along-strike (meters)"));
	slip_map_rec.add_field(5, internal::FieldDesc("area", 2, 5, "Area (square meters)"));
	slip_map_rec.add_field(6, internal::FieldDesc("mean_slip", 2, 6, "Average slip (meters)"));
	slip_map_rec.add_field(7, internal::FieldDesc("moment", 2, 7, "Seismic moment (Newton-meters)"));
	slip_map_rec.add_field(8, internal::FieldDesc("shear_before", 2, 8, "Shear stress before event (Pascal)"));
	slip_map_rec.add_field(9, internal::FieldDesc("shear_after", 2, 9, "Shear stress after event (Pascal)"));
	slip_map_rec.add_field(10, internal::FieldDesc("normal_before", 2, 10, "Normal stress before event (Pascal)"));
	slip_map_rec.add_field(11, internal::FieldDesc("normal_after", 2, 11, "Normal stress after event (Pascal)"));
	slip_map_rec.add_field(12, internal::FieldDesc("element_id", 1, 12, "Element ID number (positive integer), or negative of element count (zero if no element info)"));
	
	slip_elem_rec = internal::RecordDesc(0, "slip_element", 1, "Record 202: Slip element list entry");
	slip_elem_rec.add_field(1, internal::FieldDesc("element_id", 1, 1, "Element ID number (positive integer)"));
	
	add_record_desc_record(200, event_summary_rec);
	add_record_desc_record(201, slip_map_rec);
	add_record_desc_record(202, slip_elem_rec);
}
