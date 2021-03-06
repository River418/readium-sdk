//
//  property_extension.h
//  ePub3
//
//  Created by Jim Dovey on 2013-05-06.
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification, 
//  are permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this 
//  list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, 
//  this list of conditions and the following disclaimer in the documentation and/or 
//  other materials provided with the distribution.
//  3. Neither the name of the organization nor the names of its contributors may be 
//  used to endorse or promote products derived from this software without specific 
//  prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
//  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
//  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
//  OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef __ePub3__property_extension__
#define __ePub3__property_extension__

#include <ePub3/utilities/basic.h>
#include <ePub3/utilities/owned_by.h>
#include <ePub3/utilities/utfstring.h>
#include <ePub3/utilities/iri.h>
#include <ePub3/utilities/xml_identifiable.h>
#include <ePub3/xml/node.h>
#include <memory>

EPUB3_BEGIN_NAMESPACE

class Property;
class PropertyExtension;

typedef shared_ptr<PropertyExtension>   PropertyExtensionPtr;

///
/// An extension to an existing property, providing additional related metadata.
class PropertyExtension : public PointerType<PropertyExtension>, public OwnedBy<Property>, public XMLIdentifiable
#if EPUB_PLATFORM(WINRT)
	, public NativeBridge
#endif
{
private:
    ///
    /// No default constructor.
    PropertyExtension()                                 _DELETED_;
    ///
    /// No copy constructor.
    PropertyExtension(const PropertyExtension&)         _DELETED_;
    
public:
    /**
     Create a new Extension.
     @param node The XML element node from which this item is to be created.
     @param owner The Package to which the metadata belongs; used for property
     IRI resolution.
     */
                    PropertyExtension(const shared_ptr<Property>& owner) : OwnedBy(owner), _scheme(), _language(), _identifier() {}
    ///
    /// C++11 move constructor.
                    PropertyExtension(PropertyExtension&& o) : OwnedBy(std::move(o)), XMLIdentifiable(std::move(o)), _scheme(std::move(o._scheme)), _language(std::move(o._language)), _identifier(std::move(o._identifier)) {}
    virtual         ~PropertyExtension() {}
    
    EPUB3_EXPORT
    bool            ParseMetaElement(shared_ptr<xml::Node> node);
    
    ///
    /// Retrieves the extension's property IRI, declaring its type.
    const IRI&      PropertyIdentifier()    const           { return _identifier; }
    
    /**
     Sets the property's identifier IRI.
     @param ident The new identifier.
     */
    void            SetPropertyIdentifier(const IRI& ident) { _identifier = ident; }
    
    ///
    /// Retrieves a scheme constant which determines how the Value() is interpreted.
    const string&   Scheme()                const       { return _scheme; }
    
    /**
     Sets the property's scheme identifier.
     @param scheme The new scheme.
     */
    void            SetScheme(const string& scheme)     { _scheme = scheme; }
    
    ///
    /// The extension's value.
    const string&   Value()                 const       { return _value; }
    
    /**
     Sets the property's string value.
     @param value The new value.
     */
    void            SetValue(const string& value)       { _value = value; }
    
    ///
    /// The language of the item (if applicable).
    const string&   Language()              const       { return _language; }
    
    /**
     Sets the property's language, encoded in XML as an `xml:lang` attribute.
     @param lang The new language value, from [IETF BCP 47](http://tools.ietf.org/html/rfc4646).
     */
    void            SetLanguage(const string& lang)     { _language = lang; }
    
private:
    string      _value;
    string      _scheme;
    string      _language;
    IRI         _identifier;
};

EPUB3_END_NAMESPACE

#endif /* defined(__ePub3__property_extension__) */
